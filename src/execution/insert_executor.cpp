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
  child_executor_->Init();
  // throw NotImplementedException("InsertExecutor is not implemented");
}

auto InsertExecutor::Next([[maybe_unused]] Tuple *tuple, RID *rid) -> bool {
  if (bo == true) {
    return false;
  }
  int i = 0;
  while (child_executor_->Next(tuple, rid)) {
    Transaction *transaction = exec_ctx_->GetTransaction();
    table_oid_t table_oid = plan_->GetTableOid();
    Catalog *catalog = exec_ctx_->GetCatalog();
    TableInfo *tableinfo = catalog->GetTable(table_oid);
    TupleMeta meta;
    meta.ts_ = transaction->GetTransactionTempTs();
    meta.is_deleted_ = false;
    *rid = tableinfo->table_->InsertTuple(meta, *tuple).value();
    // transaction->AppendWriteSet(table_oid, *rid);
    auto indexes_ = catalog->GetTableIndexes(tableinfo->name_);
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
