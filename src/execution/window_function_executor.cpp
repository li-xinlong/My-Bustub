#include "execution/executors/window_function_executor.h"
#include <execution/executors/aggregation_executor.h>
#include <execution/plans/aggregation_plan.h>
#include "execution/plans/window_plan.h"
#include "storage/table/tuple.h"
namespace bustub {

WindowFunctionExecutor::WindowFunctionExecutor(ExecutorContext *exec_ctx, const WindowFunctionPlanNode *plan,
                                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {
  for (auto &[func_column_idx, window_function] : plan_->window_functions_) {
    column_ids.insert(func_column_idx);
  }
}

void WindowFunctionExecutor::Init() {
  Tuple temp;
  RID rid_temp;
  std::vector<Tuple> child_tuples;
  while (child_executor_->Next(&temp, &rid_temp)) {
    child_tuples.push_back(temp);
  }
  for (auto &window_function : plan_->window_functions_) {
    // combine partition_by and order_by
    std::vector<std::pair<OrderByType, AbstractExpressionRef>> total_orders{};
    std::vector<std::pair<OrderByType, AbstractExpressionRef>> partition_orders{};
    for (const auto &partition_by : window_function.second.partition_by_) {
      total_orders.emplace_back(OrderByType::ASC, partition_by);
      partition_orders.emplace_back(partition_by);
    }
    for (const auto &order_by : window_function.second.order_by_) {
      total_orders.push_back(order_by);
    }
    std::sort(tuples_.begin(), tuples_.end(), [&](const Tuple &a, const Tuple &b) {
      // Compare tuples based on the order_bys.
      for (const auto &order_by : total_orders) {
        const auto &type = order_by.first;
        const auto &expr = order_by.second;
        auto result_a = expr->Evaluate(&a, child_executor_->GetOutputSchema());
        auto result_b = expr->Evaluate(&b, child_executor_->GetOutputSchema());
        if (type == OrderByType::DESC) {
          if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
            return result_a.CompareGreaterThan(result_b) == CmpBool::CmpTrue;
          }
        } else {
          if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
            return result_a.CompareLessThan(result_b) == CmpBool::CmpTrue;
          }
        }
      }
      return false;
    });

    auto iter = child_tuples.begin();
    while (iter != child_tuples.end()) {
      auto upper_bound_iter = std::upper_bound(iter, child_tuples.end(), *iter, [&](const Tuple &a, const Tuple &b) {
        for (const auto &order_by : partition_orders) {
          const auto &expr = order_by.second;
          auto result_a = expr->Evaluate(&a, child_executor_->GetOutputSchema());
          auto result_b = expr->Evaluate(&b, child_executor_->GetOutputSchema());
          if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
            return result_a.CompareLessThan(result_b) == CmpBool::CmpTrue;
          }
        }
        return false;
      });
      AggregationType agg_type;
      Value default_value;
      bool isrank = GenerateAggregationType(agg_type, default_value, window_function.second.type_);
      if (!isrank) {
        const auto agg_exprs = std::vector<AbstractExpressionRef>{window_function.second.function_};
        const auto agg_types = std::vector<AggregationType>{agg_type};
        SimpleAggregationHashTable aht{agg_exprs, agg_types};
        aht.Clear();
        std::vector<AbstractExpressionRef> group_bys_ = window_function.second.partition_by_;
        AggregateKey agg_key = MakeAggregateKey(&(*iter), group_bys_, GetOutputSchema());

        // if order_by is omitted, do a global aggregation
        if (window_function.second.order_by_.empty()) {
          AbstractExpressionRef agg_expr = window_function.second.function_;
          for (auto iter_temp = iter; iter_temp != upper_bound_iter; ++iter_temp) {
            AggregateValue agg_value = MakeAggregateValue(&(*iter_temp), agg_expr, GetOutputSchema());
            aht.InsertCombine(agg_key, agg_value);
          }
        }
        for (auto iter_temp = iter; iter_temp != upper_bound_iter; ++iter_temp) {
          auto tuple_iter = tuples_.begin() + std::distance(child_tuples.begin(), iter);

          // if order_by is valid, do a moving aggregation
          if (!window_function.second.order_by_.empty()) {
            AbstractExpressionRef agg_expr = window_function.second.function_;
            AggregateValue agg_value = MakeAggregateValue(&(*iter_temp), agg_expr, GetOutputSchema());
            aht.InsertCombine(agg_key, agg_value);
          }
          std::vector<Value> values{};
          for (uint32_t idx = 0; idx < GetOutputSchema().GetColumns().size(); idx++) {
            if (idx == window_function.first) {
              values.push_back(aht.Begin().Val().aggregates_.begin()[0]);
            } else if (column_ids.count(idx) == 0) {
              values.push_back(plan_->columns_[idx]->Evaluate(&(*iter), child_executor_->GetOutputSchema()));
            } else if (tuple_iter < tuples_.end()) {
              values.push_back(tuple_iter->GetValue(&GetOutputSchema(), idx));
            } else {
              values.push_back(default_value);
            }
          }

          if (tuple_iter < tuples_.end()) {
            *tuple_iter = Tuple{values, &GetOutputSchema()};
          } else {
            tuples_.emplace_back(values, GetOutputSchema());
          }
        }

      } else {
        int global_rank = 0;
        int local_rank = 0;
        for (auto iter_temp = iter; iter_temp != upper_bound_iter; ++iter_temp) {
          auto tuple_iter = tuples_.begin() + std::distance(child_tuples.begin(), iter);

          std::vector<Value> values{};
          for (uint32_t idx = 0; idx < GetOutputSchema().GetColumns().size(); idx++) {
            if (idx == window_function.first) {
              ++global_rank;
              if (local_rank == 0 || !Equal(*iter, *(iter - 1), window_function.second.order_by_, GetOutputSchema())) {
                local_rank = global_rank;
              }
              values.push_back(ValueFactory::GetIntegerValue(local_rank));
            } else if (column_ids.count(idx) == 0) {
              values.push_back(plan_->columns_[idx]->Evaluate(&(*iter), child_executor_->GetOutputSchema()));
            } else if (tuple_iter < tuples_.end()) {
              values.push_back(tuple_iter->GetValue(&GetOutputSchema(), idx));
            } else {
              values.push_back(default_value);
            }
          }

          if (tuple_iter < tuples_.end()) {
            *tuple_iter = Tuple{values, &GetOutputSchema()};
          } else {
            tuples_.emplace_back(values, GetOutputSchema());
          }
        }
      }
    }
  }
  // NotImplementedException("WindowFunctionExecutor is not implemented");
}
auto Equal(const Tuple &a, const Tuple &b, const std::vector<std::pair<OrderByType, AbstractExpressionRef>> order_bys_,
           const Schema &output_schema_) -> bool {
  for (const auto &order_key : order_bys_) {
    switch (order_key.first) {
      case OrderByType::INVALID:  // NOLINT
      case OrderByType::DEFAULT:  // NOLINT
      case OrderByType::ASC:
        return static_cast<bool>(order_key.second->Evaluate(&a, output_schema_)
                                     .CompareEquals(order_key.second->Evaluate(&b, output_schema_)));
      case OrderByType::DESC:
        return static_cast<bool>(order_key.second->Evaluate(&a, output_schema_)
                                     .CompareEquals(order_key.second->Evaluate(&b, output_schema_)));
    }
  }
  return false;
}
auto WindowFunctionExecutor::MakeAggregateKey(const Tuple *tuple, std::vector<AbstractExpressionRef> &group_bys_,
                                              const Schema &output_schema_) -> AggregateKey {
  std::vector<Value> keys;
  for (const auto &expr : group_bys_) {
    keys.emplace_back(expr->Evaluate(tuple, output_schema_));
  }
  return {keys};
}
auto WindowFunctionExecutor::MakeAggregateValue(const Tuple *tuple, AbstractExpressionRef &agg_expr_,
                                                const Schema &output_schema_) -> AggregateValue {
  std::vector<Value> vals;
  vals.emplace_back(agg_expr_->Evaluate(tuple, output_schema_));
  return {vals};
}
auto WindowFunctionExecutor::GenerateAggregationType(AggregationType &agg_type, Value &default_value,
                                                     WindowFunctionType window_function_type) -> bool {
  bool is_rank = false;

  switch (window_function_type) {
    case WindowFunctionType::CountStarAggregate:
      agg_type = AggregationType::CountStarAggregate;
      default_value = ValueFactory::GetIntegerValue(0);
      break;
    case WindowFunctionType::CountAggregate:
      agg_type = AggregationType::CountAggregate;
      default_value = ValueFactory::GetNullValueByType(TypeId::INTEGER);
      break;
    case WindowFunctionType::SumAggregate:
      agg_type = AggregationType::SumAggregate;
      default_value = ValueFactory::GetNullValueByType(TypeId::INTEGER);
      break;
    case WindowFunctionType::MinAggregate:
      agg_type = AggregationType::MinAggregate;
      default_value = ValueFactory::GetNullValueByType(TypeId::INTEGER);
      break;
    case WindowFunctionType::MaxAggregate:
      agg_type = AggregationType::MaxAggregate;
      default_value = ValueFactory::GetNullValueByType(TypeId::INTEGER);
      break;
    case WindowFunctionType::Rank:
      is_rank = true;
      default_value = ValueFactory::GetNullValueByType(TypeId::INTEGER);
      break;
  }
  return is_rank;
}

auto WindowFunctionExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (iterator_ == tuples_.end()) {
    return false;
  }
  Tuple tuple_temp = *iterator_;
  *tuple = tuple_temp;
  *rid = tuple_temp.GetRid();
  ++iterator_;
  return false;
}
}  // namespace bustub
