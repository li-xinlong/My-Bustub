//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// seq_scan_executor.cpp
//
// Identification: src/execution/seq_scan_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "execution/executors/seq_scan_executor.h"

namespace bustub {

SeqScanExecutor::SeqScanExecutor(ExecutorContext *exec_ctx, const SeqScanPlanNode *plan) : AbstractExecutor(exec_ctx) {
  plan_ = plan;
}

void SeqScanExecutor::Init() {
  Catalog *catalog = exec_ctx_->GetCatalog();
  table_oid_t table_oid = plan_->GetTableOid();
  TableInfo *tableinfo = catalog->GetTable(table_oid);
  auto &tableheap = tableinfo->table_;
  iterator_ = std::make_unique<TableIterator>(tableheap->MakeIterator());
  //   throw NotImplementedException("SeqScanExecutor is not implemented");
}

auto SeqScanExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (iterator_->IsEnd()) {
    return false;
  } else {
    while (!iterator_->IsEnd()) {
      std::pair<TupleMeta, Tuple> temp = iterator_->GetTuple();
      ++(*iterator_);
      if (temp.first.is_deleted_) {
        continue;
      } else {
        *tuple = temp.second;
        *rid = temp.second.GetRid();
        return true;
      }
    }
    return false;
  }
}

}  // namespace bustub
