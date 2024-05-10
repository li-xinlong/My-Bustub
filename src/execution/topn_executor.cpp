#include "execution/executors/topn_executor.h"

namespace bustub {

TopNExecutor::TopNExecutor(ExecutorContext *exec_ctx, const TopNPlanNode *plan,
                           std::unique_ptr<AbstractExecutor> &&child_executor)
    : AbstractExecutor(exec_ctx), plan_(plan), child_executor_(std::move(child_executor)) {}

void TopNExecutor::Init() {
  child_executor_->Init();
  auto cmp = [order_bys = plan_->order_bys_, schema = child_executor_->GetOutputSchema()](const Tuple &a,
                                                                                          const Tuple &b) {
    for (const auto &order_by : order_bys) {
      const auto &type = order_by.first;
      const auto &expr = order_by.second;
      auto result_a = expr->Evaluate(&a, schema);
      auto result_b = expr->Evaluate(&b, schema);
      if (type == OrderByType::DESC) {
        if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
          return result_a.CompareGreaterThan(result_b) == CmpBool::CmpTrue;
        }
      } else {
        if (result_a.CheckComparable(result_b) && result_a.CompareNotEquals(result_b) == CmpBool::CmpTrue) {
          return result_a.CompareLessThan(result_b) == CmpBool::CmpTrue;
        }
      }
    }
    return false;
  };
  std::priority_queue<Tuple, std::vector<Tuple>, decltype(cmp)> priority_queue(cmp);
  Tuple child_tuple{};
  RID child_rid;
  while (child_executor_->Next(&child_tuple, &child_rid)) {
    priority_queue.push(child_tuple);
    if (priority_queue.size() > plan_->GetN()) {
      priority_queue.pop();
    }
  }
  while (!priority_queue.empty()) {
    order_topn_stack.push(priority_queue.top());
    priority_queue.pop();
  }
  // throw NotImplementedException("TopNExecutor is not implemented");
}

auto TopNExecutor::Next(Tuple *tuple, RID *rid) -> bool {
  if (order_topn_stack.empty()) {
    return false;
  }
  *tuple = order_topn_stack.top();
  *rid = tuple->GetRid();
  order_topn_stack.pop();
  return true;
}

auto TopNExecutor::GetNumInHeap() -> size_t {
  return plan_->GetN();
  // throw NotImplementedException("TopNExecutor is not implemented");
};

}  // namespace bustub
