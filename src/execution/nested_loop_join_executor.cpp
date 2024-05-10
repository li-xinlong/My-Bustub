//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// nested_loop_join_executor.cpp
//
// Identification: src/execution/nested_loop_join_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#include "execution/executors/nested_loop_join_executor.h"
#include <type/value_factory.h>
#include "binder/table_ref/bound_join_ref.h"
#include "common/exception.h"

namespace bustub {

NestedLoopJoinExecutor::NestedLoopJoinExecutor(ExecutorContext *exec_ctx, const NestedLoopJoinPlanNode *plan,
                                               std::unique_ptr<AbstractExecutor> &&left_executor,
                                               std::unique_ptr<AbstractExecutor> &&right_executor)
    : AbstractExecutor(exec_ctx),
      plan_(plan),
      left_executor_(std::move(left_executor)),
      right_executor_((std::move(right_executor))) {
  if (!(plan->GetJoinType() == JoinType::LEFT || plan->GetJoinType() == JoinType::INNER)) {
    // Note for 2023 Fall: You ONLY need to implement left join and inner join.
    throw bustub::NotImplementedException(fmt::format("join type {} not supported", plan->GetJoinType()));
  }
}

auto NestedLoopJoinExecutor::BuildLeftJoinTuple(Tuple *left_tuple) -> Tuple {
  std::vector<Value> values;
  values.reserve(GetOutputSchema().GetColumnCount());
  Tuple tuple;
  for (size_t i = 0; i < left_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(left_tuple->GetValue(&left_executor_->GetOutputSchema(), i));
  }
  for (size_t i = 0; i < right_executor_->GetOutputSchema().GetColumnCount(); i++) {
    const auto &column_type = plan_->GetRightPlan()->OutputSchema().GetColumn(i).GetType();
    values.emplace_back(ValueFactory::GetNullValueByType(column_type));
  }
  tuple = Tuple(values, &(GetOutputSchema()));
  return tuple;
}
auto NestedLoopJoinExecutor::BuildInnerJoinTuple(Tuple *left_tuple, Tuple *right_tuple) -> Tuple {
  std::vector<Value> values;
  values.reserve(GetOutputSchema().GetColumnCount());
  Tuple tuple;

  for (size_t i = 0; i < left_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(left_tuple->GetValue(&left_executor_->GetOutputSchema(), i));
  }
  for (size_t i = 0; i < right_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(right_tuple->GetValue(&right_executor_->GetOutputSchema(), i));
  }
  tuple = Tuple(values, &(GetOutputSchema()));
  return tuple;
}
void NestedLoopJoinExecutor::Init() {
  left_executor_.get()->Init();
  right_executor_.get()->Init();
  while (left_executor_->Next(&left_tuple, &left_rid)) {
    left_tuple_vector.emplace_back(left_tuple);
  }
  is_have_right_tuple = right_executor_->Next(&right_tuple, &right_rid);
  // throw NotImplementedException("NestedLoopJoinExecutor is not implemented");
}

auto NestedLoopJoinExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  for (; i < left_tuple_vector.size(); i++, left_have_right = 0) {
    left_tuple = left_tuple_vector[i];
    while (is_have_right_tuple) {
      if (plan_->GetJoinType() == JoinType::INNER) {
        auto evaluateJoinRet = plan_->Predicate()->EvaluateJoin(&left_tuple, left_executor_->GetOutputSchema(),
                                                                &right_tuple, right_executor_->GetOutputSchema());
        if (!evaluateJoinRet.IsNull() && evaluateJoinRet.GetAs<bool>()) {
          *tuple = BuildInnerJoinTuple(&left_tuple, &right_tuple);
          *rid = tuple->GetRid();
          is_have_right_tuple = right_executor_->Next(&right_tuple, &right_rid);
          return true;
        }
      } else if (plan_->GetJoinType() == JoinType::LEFT) {
        auto evaluateJoinRet = plan_->Predicate()->EvaluateJoin(&left_tuple, left_executor_->GetOutputSchema(),
                                                                &right_tuple, right_executor_->GetOutputSchema());
        if (!evaluateJoinRet.IsNull() && evaluateJoinRet.GetAs<bool>()) {
          *tuple = BuildInnerJoinTuple(&left_tuple, &right_tuple);
          *rid = tuple->GetRid();
          size = i + 1;
          is_have_right_tuple = right_executor_->Next(&right_tuple, &right_rid);
          left_have_right = 1;
          return true;
        }
      }
      is_have_right_tuple = right_executor_->Next(&right_tuple, &right_rid);
    }
    right_executor_->Init();
    is_have_right_tuple = right_executor_->Next(&right_tuple, &right_rid);
    if (!left_have_right && plan_->GetJoinType() == JoinType::LEFT) {
      left_tuple = left_tuple_vector[i];
      *tuple = BuildLeftJoinTuple(&left_tuple);
      *rid = tuple->GetRid();
      i++;
      return true;
    }
  }
  return false;
}

}  // namespace bustub
