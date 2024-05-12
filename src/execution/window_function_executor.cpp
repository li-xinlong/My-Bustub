#include "execution/executors/window_function_executor.h"
#include <execution/executors/aggregation_executor.h>
#include <execution/plans/aggregation_plan.h>
#include "execution/plans/window_plan.h"
#include "storage/table/tuple.h"
namespace bustub {

WindowFunctionExecutor::WindowFunctionExecutor(ExecutorContext *exec_ctx, const WindowFunctionPlanNode *plan,
                                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {
  // for (auto &[func_column_idx, window_function] : plan_->window_functions_) {
  //   column_ids.insert(func_column_idx);
  // }
}

void WindowFunctionExecutor::Init() {
  // child_executor_->Init();
  // Tuple child_tuple{};
  // RID rid;
  // std::vector<Tuple> child_tuples;
  // while (child_executor_->Next(&child_tuple, &rid)) {
  //   tuples_.push_back(child_tuple);
  // }
  // for (auto &windouws_function : plan_->window_functions_) {
  //   auto function_id = windouws_function.first;
  //   auto function = windouws_function.second;
  //   std::vector<std::pair<OrderByType, AbstractExpressionRef>> order_bys{};
  //   std::vector<AbstractExpressionRef> partition_bys{};
  //   for (const auto &partition_by : function.partition_by_) {
  //     order_bys.emplace_back(OrderByType::ASC, partition_by);
  //     partition_bys.emplace_back(partition_by);
  //   }
  //   for (const auto &order_by : function.order_by_) {
  //     order_bys.emplace_back(order_by);
  //   }
  //   std::sort(child_tuples.begin(), child_tuples.end(), [&](const Tuple &a, const Tuple &b) {
  //     // Compare tuples based on the order_bys.
  //     for (const auto &order_by : order_bys) {
  //       const auto &type = order_by.first;
  //       const auto &expr = order_by.second;
  //       auto result_a = expr->Evaluate(&a, child_executor_->GetOutputSchema());
  //       auto result_b = expr->Evaluate(&b, child_executor_->GetOutputSchema());
  //       if (type == OrderByType::DESC) {
  //         if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
  //           return result_a.CompareGreaterThan(result_b) == CmpBool::CmpTrue;
  //         }
  //       } else {
  //         if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
  //           return result_a.CompareLessThan(result_b) == CmpBool::CmpTrue;
  //         }
  //       }
  //     }
  //     return false;
  //   });
  //   // WindowAggregate wind_agg{plan_->columns_,
  //   //                          function.partition_by_,
  //   //                          function.function_,
  //   //                          function_id,
  //   //                          column_ids,
  //   //                          function.type_,
  //   //                          function.order_by_,
  //   //                          child_executor_->GetOutputSchema(),
  //   //                          plan_->OutputSchema(),
  //   //                          tuples_};
  //   auto iter = child_tuples.begin();
  //   while (iter != child_tuples.end()) {
  //     auto upper_bound_iter = std::upper_bound(iter, child_tuples.end(), *iter, [&](const Tuple &a, const Tuple &b) {
  //       for (const auto &order_by : partition_bys) {
  //         const auto &expr = order_by;
  //         auto result_a = expr->Evaluate(&a, child_executor_->GetOutputSchema());
  //         auto result_b = expr->Evaluate(&b, child_executor_->GetOutputSchema());
  //         if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
  //           return result_a.CompareLessThan(result_b) == CmpBool::CmpTrue;
  //         }
  //       }
  //       return false;
  //     });
  //     AggregationType aggregationtype;
  //     Value value_begin;
  //     bool is_rank;
  //     whatType(function.type_, aggregationtype, value_begin, is_rank);

  //     if (is_rank) {
  //       SimpleAggregationHashTable aht(std::vector<AbstractExpressionRef>{function.function_},
  //                                      std::vector<AggregationType>{aggregationtype});
  //       aht.Clear();
  //       AggregateKey agg_key = MakeAggregateKey(&(*iter), function.partition_by_);
  //       if (function.order_by_.empty()) {
  //         for (auto iter_temp = iter; iter_temp != upper_bound_iter; ++iter_temp) {
  //           AggregateValue agg_value = MakeAggregateValue(&(*iter_temp), function.function_);
  //           aht.InsertCombine(agg_key, agg_value);
  //         }
  //       }
  //       for (auto iter_temp = iter; iter_temp != upper_bound_iter; ++iter_temp) {
  //         auto tuple_iter = tuples_.begin() + std::distance(child_tuples.begin(), iter_temp);

  //         // if order_by is valid, do a moving aggregation
  //         if (!function.order_by_.empty()) {
  //           AggregateValue agg_value = MakeAggregateValue(&(*iter_temp), function.function_);
  //           aht.InsertCombine(agg_key, agg_value);
  //         }

  //         std::vector<Value> values{};
  //         for (uint32_t idx = 0; idx < plan_->OutputSchema().GetColumns().size(); idx++) {
  //           if (idx == function_id) {
  //             values.push_back(aht.Begin().Val().aggregates_.begin()[0]);
  //           } else if (column_ids.count(idx) == 0) {
  //             values.push_back(columns_[idx]->Evaluate(&(*iter), *child_schema_));
  //           } else if (tuple_iter < tuples_.end()) {
  //             values.push_back(tuple_iter->GetValue(plan_->OutputSchema(), idx));
  //           } else {
  //             values.push_back(value_begin);
  //           }
  //         }
  //       }
  //       iter = upper_bound_iter;
  //     }
  //     iterator_ = tuples_.begin();
  NotImplementedException("WindowFunctionExecutor is not implemented");
  //   }
  // }
}

// auto WindowFunctionExecutor::whatType(WindowFunctionType &function_type, AggregationType &aggregationtype,
//                                       Value &value_begin, bool &is_rank) {
//   switch (function_type) {
//     case WindowFunctionType::CountStarAggregate:
//       aggregationtype = AggregationType::CountStarAggregate;
//       value_begin = ValueFactory::GetIntegerValue(0);
//       break;
//     case WindowFunctionType::CountAggregate:
//       aggregationtype = AggregationType::CountAggregate;
//       value_begin = ValueFactory::GetNullValueByType(TypeId::INTEGER);
//       break;
//     case WindowFunctionType::SumAggregate:
//       aggregationtype = AggregationType::SumAggregate;
//       value_begin = ValueFactory::GetNullValueByType(TypeId::INTEGER);
//       break;
//     case WindowFunctionType::MinAggregate:
//       aggregationtype = AggregationType::MinAggregate;
//       value_begin = ValueFactory::GetNullValueByType(TypeId::INTEGER);
//       break;
//     case WindowFunctionType::MaxAggregate:
//       aggregationtype = AggregationType::MaxAggregate;
//       value_begin = ValueFactory::GetNullValueByType(TypeId::INTEGER);
//       break;
//     default:
//       is_rank = false;
//       value_begin = ValueFactory::GetNullValueByType(TypeId::INTEGER);
//       break;
//   }
// }
auto WindowFunctionExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  // if (iterator_ == tuples_.end()) {
  //   return false;
  // }
  // Tuple tuple_temp = *iterator_;
  // *tuple = tuple_temp;
  // *rid = tuple_temp.GetRid();
  // ++iterator_;
  return false;
}
}  // namespace bustub
