//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// index_scan_executor.cpp
//
// Identification: src/execution/index_scan_executor.cpp
//
// Copyright (c) 2015-19, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#include "execution/executors/index_scan_executor.h"

namespace bustub {
IndexScanExecutor::IndexScanExecutor(ExecutorContext *exec_ctx, const IndexScanPlanNode *plan)
    : AbstractExecutor(exec_ctx) {
  plan_ = plan;
}

void IndexScanExecutor::Init() {
  Catalog *catalog = exec_ctx_->GetCatalog();
  index_oid_t index_oid = plan_->GetIndexOid();
  index_info_ = catalog->GetIndex(index_oid);
  table_info_ = catalog->GetTable(index_info_->table_name_);
  htable_ = dynamic_cast<HashTableIndexForTwoIntegerColumn *>(index_info_->index_.get());
  Transaction *transaction = exec_ctx_->GetTransaction();
  std::vector<RID> rids;
  for (auto key : plan_->pred_keys_) {
    std::vector<RID> rids_temp;
    Tuple tuple;
    std::vector<Column> column;
    std::vector<Value> values;
    Schema schema(column);
    auto value = key->Evaluate(&tuple, schema);
    column.emplace_back(value.GetColumn());
    values.emplace_back(value);
    Tuple key_tuple{values, &schema};
    htable_->ScanKey(key_tuple, &rids_temp, transaction);
    for (auto rid : rids_temp) {
      rids.push_back(rid);
    }
  }
  iterator_ = rids.begin();
  // throw NotImplementedException("IndexScanExecutor is not implemented");
}

auto IndexScanExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (iterator_ == rids.end()) {
    return false;
  } else {
    while (iterator_ != rids.end()) {
      RID rid_temp = *iterator_;
      *iterator_++;
      Tuple tuple_temp = table_info_->table_->GetTuple(rid_temp).second;
      TupleMeta meta = table_info_->table_->GetTuple(rid_temp).first;
      if (meta.is_deleted_) {
        continue;
      } else {
        if (plan_->filter_predicate_ == nullptr ||
            plan_->filter_predicate_->Evaluate(&tuple_temp, table_info_->schema_).GetAs<bool>()) {
          *tuple = tuple_temp;
          *rid = tuple_temp.GetRid();
          return true;
        }
      }
    }
    return false;
  }
  // return false;
}

}  // namespace bustub
