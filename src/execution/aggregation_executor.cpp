//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// aggregation_executor.cpp
//
// Identification: src/execution/aggregation_executor.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//
#include <memory>
#include <vector>

#include "execution/executors/aggregation_executor.h"

namespace bustub {

AggregationExecutor::AggregationExecutor(ExecutorContext *exec_ctx, const AggregationPlanNode *plan,
                                         std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx),
      plan_(plan),
      child_executor_(std::move(child_executor)),
      aht_(plan->GetAggregates(), plan->GetAggregateTypes()),
      aht_iterator_(aht_.Begin()) {}

void AggregationExecutor::Init() {
  aht_.Clear();
  Tuple tuple{};
  RID rid{};
  int i = 0;
  child_executor_->Init();
  while (child_executor_->Next(&tuple, &rid)) {
    i++;
    AggregateKey key_temp = MakeAggregateKey(&tuple);
    AggregateValue val_temp = MakeAggregateValue(&tuple);
    aht_.InsertCombine(key_temp, val_temp);
  }
  aht_iterator_ = aht_.Begin();
  if (aht_iterator_ == aht_.End() && GetOutputSchema().GetColumnCount() == 1) {
    bo = true;
  }
}

auto AggregationExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (bo) {
    *tuple = Tuple{std::move(aht_.GenerateInitialAggregateValue().aggregates_), &GetOutputSchema()};
    *rid = tuple->GetRid();
    bo = false;
    return true;
  }
  if (aht_iterator_ == aht_.End()) {
    return false;
  } else {
    std::vector<Value> values{};
    values.reserve(GetOutputSchema().GetColumnCount());

    values.insert(values.end(), aht_iterator_.Key().group_bys_.begin(), aht_iterator_.Key().group_bys_.end());

    values.insert(values.end(), aht_iterator_.Val().aggregates_.begin(), aht_iterator_.Val().aggregates_.end());
    *tuple = Tuple{values, &GetOutputSchema()};
    *rid = tuple->GetRid();

    ++aht_iterator_;
    return true;
  }
}

auto AggregationExecutor::GetChildExecutor() const -> const AbstractExecutor * { return child_executor_.get(); }

}  // namespace bustub
