//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// delete_executor.cpp
//
// Identification: src/execution/delete_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <memory>

#include "execution/executors/delete_executor.h"

namespace bustub {

DeleteExecutor::DeleteExecutor(ExecutorContext *exec_ctx, const DeletePlanNode *plan,
                               std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx) {
  plan_ = plan;
  child_executor_ = std::move(child_executor);
}

void DeleteExecutor::Init() {
  child_executor_->Init();
  //  throw NotImplementedException("DeleteExecutor is not implemented");
}

auto DeleteExecutor::Next([[maybe_unused]] Tuple *tuple, RID *rid) -> bool {
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
    meta.is_deleted_ = true;
    tableinfo->table_->UpdateTupleMeta(meta, *rid);
    // transaction->AppendWriteSet(table_oid, *rid);
    auto indexes_ = catalog->GetTableIndexes(tableinfo->name_);
    for (auto it = indexes_.begin(); it != indexes_.end(); ++it) {
      IndexInfo *index_temp = *it;
      index_temp->index_->DeleteEntry(
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
