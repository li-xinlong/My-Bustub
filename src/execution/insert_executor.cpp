//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// insert_executor.cpp
//
// Identification: src/execution/insert_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <memory>

#include "execution/executors/insert_executor.h"

namespace bustub {

InsertExecutor::InsertExecutor(ExecutorContext *exec_ctx, const InsertPlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx) {
  plan_ = plan;
  child_executor_ = std::move(child_executor);
}

void InsertExecutor::Init() {
  Catalog *catalog = exec_ctx_->GetCatalog();
  table_oid_t table_oid = plan_->GetTableOid();
  TableInfo *tableinfo = catalog->GetTable(table_oid);
  table_heap_ = std::move(tableinfo->table_);
  indexes_ = catalog->GetTableIndexes(tableinfo->name_);
  child_executor_->Init();
  //   throw NotImplementedException("InsertExecutor is not implemented");
}

auto InsertExecutor::Next([[maybe_unused]] Tuple *tuple, RID *rid) -> bool {
  if (!child_executor_->Next(tuple, rid)) {
    return false;
  }

  Transaction *transaction = exec_ctx_->GetTransaction();
  LockManager *lockmar = exec_ctx_->GetLockManager();
  table_oid_t table_oid = plan_->GetTableOid();

  TupleMeta meta;
  meta.ts_ = ts++;
  meta.is_deleted_ = false;
  table_heap_->InsertTuple(meta, *tuple, lockmar, transaction, table_oid);
  for (auto it = indexes_.begin(); it != indexes_.end(); ++it) {
  }
  return true;
}
}  // namespace bustub
