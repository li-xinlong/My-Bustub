//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// update_executor.cpp
//
// Identification: src/execution/update_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#include <memory>

#include "execution/executors/update_executor.h"

namespace bustub {

UpdateExecutor::UpdateExecutor(ExecutorContext *exec_ctx, const UpdatePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx) {
  plan_ = plan;
  child_executor_ = std::move(child_executor);
  // As of Fall 2022, you DON'T need to implement update executor to have perfect score in project 3 / project 4.
}

void UpdateExecutor::Init() {
  child_executor_->Init();
  Catalog *catalog = exec_ctx_->GetCatalog();
  table_oid_t table_oid = plan_->GetTableOid();
  table_info_ = catalog->GetTable(table_oid);
  // throw NotImplementedException("UpdateExecutor is not implemented");
}

auto UpdateExecutor::Next([[maybe_unused]] Tuple *tuple, RID *rid) -> bool {
  if (bo == true) {
    return false;
  }
  int i = 0;
  while (child_executor_->Next(tuple, rid)) {
    Transaction *transaction = exec_ctx_->GetTransaction();
    LockManager *lockmar = exec_ctx_->GetLockManager();
    table_oid_t table_oid = plan_->GetTableOid();
    Catalog *catalog = exec_ctx_->GetCatalog();
    TableInfo *tableinfo = catalog->GetTable(table_oid);
    TupleMeta meta;
    meta.ts_ = transaction->GetTransactionTempTs();
    meta.is_deleted_ = true;
    tableinfo->table_->UpdateTupleMeta(meta, *rid);

    meta.ts_ = transaction->GetTransactionTempTs();
    meta.is_deleted_ = false;
    std::vector<Value> values;
    values.reserve(plan_->target_expressions_.size());
    for (const auto &expression : plan_->target_expressions_) {
      values.emplace_back(expression->Evaluate(tuple, table_info_->schema_));
    }
    Tuple tupe_temp = Tuple{values, &table_info_->schema_};
    *rid = tableinfo->table_->InsertTuple(meta, tupe_temp, lockmar, transaction, table_oid).value();
    // transaction->AppendWriteSet(table_oid, *rid);
    auto indexes_ = catalog->GetTableIndexes(table_info_->name_);
    for (auto it = indexes_.begin(); it != indexes_.end(); ++it) {
      IndexInfo *index_temp = *it;
      index_temp->index_->InsertEntry(
          tuple->KeyFromTuple(tableinfo->schema_, index_temp->key_schema_, index_temp->index_->GetKeyAttrs()), *rid,
          transaction);
    }
    i++;
  }
  *tuple = Tuple{std::vector<Value>{{TypeId::INTEGER, i}}, &GetOutputSchema()};
  *rid = RID{};
  bo = true;
  return true;
}

}  // namespace bustub
