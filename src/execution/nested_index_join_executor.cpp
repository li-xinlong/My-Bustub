//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// nested_index_join_executor.cpp
//
// Identification: src/execution/nested_index_join_executor.cpp
//
// Copyright (c) 2015-19, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "execution/executors/nested_index_join_executor.h"
#include <type/value_factory.h>
namespace bustub {

NestIndexJoinExecutor::NestIndexJoinExecutor(ExecutorContext *exec_ctx, const NestedIndexJoinPlanNode *plan,
                                             std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {
  if (!(plan->GetJoinType() == JoinType::LEFT || plan->GetJoinType() == JoinType::INNER)) {
    // Note for 2023 Spring: You ONLY need to implement left join and inner join.
    throw bustub::NotImplementedException(fmt::format("join type {} not supported", plan->GetJoinType()));
  }
}

void NestIndexJoinExecutor::Init() {
  child_executor_->Init();
  Catalog *catalog = exec_ctx_->GetCatalog();
  index_oid_t index_oid = plan_->GetIndexOid();
  index_info_ = catalog->GetIndex(index_oid);
  table_info_ = catalog->GetTable(index_info_->table_name_);
  htable_ = dynamic_cast<HashTableIndexForTwoIntegerColumn *>(index_info_->index_.get());
  // throw NotImplementedException("NestIndexJoinExecutor is not implemented");
}

auto NestIndexJoinExecutor::BuildLeftJoinTuple(Tuple *left_tuple) -> Tuple {
  std::vector<Value> values;
  values.reserve(GetOutputSchema().GetColumnCount());
  Tuple tuple;
  for (size_t i = 0; i < child_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(left_tuple->GetValue(&child_executor_->GetOutputSchema(), i));
  }
  for (size_t i = 0; i < child_executor_->GetOutputSchema().GetColumnCount(); i++) {
    const auto &column_type = plan_->OutputSchema().GetColumn(i).GetType();
    values.emplace_back(ValueFactory::GetNullValueByType(column_type));
  }
  tuple = Tuple(values, &(GetOutputSchema()));
  return tuple;
}
auto NestIndexJoinExecutor::BuildInnerJoinTuple(Tuple *left_tuple, Tuple *right_tuple) -> Tuple {
  std::vector<Value> values;
  values.reserve(GetOutputSchema().GetColumnCount());
  Tuple tuple;

  for (size_t i = 0; i < child_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(left_tuple->GetValue(&child_executor_->GetOutputSchema(), i));
  }
  for (size_t i = 0; i < child_executor_->GetOutputSchema().GetColumnCount(); i++) {
    values.emplace_back(right_tuple->GetValue(&child_executor_->GetOutputSchema(), i));
  }
  tuple = Tuple(values, &(GetOutputSchema()));
  return tuple;
}
auto NestIndexJoinExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  Tuple tuple_temp;
  RID rid_temp;
  while (child_executor_->Next(&tuple_temp, &rid_temp)) {
    // make search key for index
    auto key_value = plan_->KeyPredicate().get()->Evaluate(&tuple_temp, child_executor_->GetOutputSchema());
    std::vector<Value> key_values{key_value};
    Tuple key_tuple(key_values, &index_info_->key_schema_);
    std::vector<bustub::RID> scan_rid;
    htable_->ScanKey(key_tuple, &scan_rid, exec_ctx_->GetTransaction());
    if (!scan_rid.empty()) {
      auto right = table_info_->table_->GetTuple(scan_rid[0]);
      Tuple right_tuple = right.second;
      *tuple = BuildInnerJoinTuple(&tuple_temp, &right_tuple);
      *rid = tuple->GetRid();
      return true;
    } else {
      if (plan_->GetJoinType() == JoinType::LEFT) {
        *tuple = BuildLeftJoinTuple(&tuple_temp);
        *rid = tuple->GetRid();
        return true;
      }
    }
  }
  return false;
}

}  // namespace bustub
