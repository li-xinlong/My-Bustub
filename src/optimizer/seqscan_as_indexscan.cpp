#include <binder/tokens.h>
#include <execution/expressions/constant_value_expression.h>
#include <execution/plans/index_scan_plan.h>
#include <execution/plans/seq_scan_plan.h>
#include "execution/expressions/column_value_expression.h"
#include "execution/expressions/comparison_expression.h"
#include "execution/expressions/constant_value_expression.h"
#include "optimizer/optimizer.h"
namespace bustub {

auto Optimizer::OptimizeSeqScanAsIndexScan(const bustub::AbstractPlanNodeRef &plan) -> AbstractPlanNodeRef {
  // TODO(student): implement seq scan with predicate -> index scan optimizer rule
  // The Filter Predicate Pushdown has been enabled for you in optimizer.cpp when forcing starter rule
  std::vector<AbstractPlanNodeRef> children;
  for (const auto &child : plan->GetChildren()) {
    children.emplace_back(OptimizeSeqScanAsIndexScan(child));
  }
  auto optimized_plan = plan->CloneWithChildren(std::move(children));
  if (optimized_plan->GetType() == PlanType::SeqScan) {
    const auto &seq_scan_plan = dynamic_cast<const SeqScanPlanNode &>(*optimized_plan);
    BUSTUB_ASSERT(optimized_plan->children_.empty(), "must have no children");
    const ComparisonExpression *expr =
        dynamic_cast<const ComparisonExpression *>(seq_scan_plan.filter_predicate_.get());
    if (expr != nullptr) {
      if (expr->comp_type_ == ComparisonType::Equal) {
        auto column_value_expr1 = dynamic_cast<const ColumnValueExpression *>(expr->GetChildAt(0).get());
        if (column_value_expr1 != nullptr) {
          auto constant_expr1 = dynamic_cast<ConstantValueExpression *>(expr->GetChildAt(1).get());
          if (constant_expr1 != nullptr) {
            const auto &column_name = catalog_.GetTable(seq_scan_plan.table_oid_)
                                          ->schema_.GetColumn(column_value_expr1->GetColIdx())
                                          .GetName();
            // if exist an index on this column
            const std::vector<IndexInfo *> &table_indexes = catalog_.GetTableIndexes(seq_scan_plan.table_name_);
            for (const auto &index_info : table_indexes) {
              BUSTUB_ASSERT(index_info->index_->GetKeySchema()->GetColumnCount() == 1,
                            "key_schema must only has 1 column");
              if (column_name == index_info->index_->GetKeySchema()->GetColumn(0).GetName()) {
                std::vector<AbstractExpressionRef> pred_keys;
                pred_keys.emplace_back((AbstractExpressionRef)expr->GetChildAt(0).get());
                return std::make_shared<IndexScanPlanNode>(seq_scan_plan.output_schema_, seq_scan_plan.table_oid_,
                                                           index_info->index_oid_, seq_scan_plan.filter_predicate_,
                                                           pred_keys);
              }
            }
          }
        }
        // case 2: left side is constant, right side is column
        auto constant_expr2 = dynamic_cast<ConstantValueExpression *>(expr->GetChildAt(0).get());
        if (constant_expr2 != nullptr) {
          auto column_value_expr2 = dynamic_cast<const ColumnValueExpression *>(expr->GetChildAt(1).get());
          if (column_value_expr2 != nullptr) {
            const auto &column_name = catalog_.GetTable(seq_scan_plan.table_oid_)
                                          ->schema_.GetColumn(column_value_expr2->GetColIdx())
                                          .GetName();

            // if exist an index on this column
            const std::vector<IndexInfo *> &table_indexes = catalog_.GetTableIndexes(seq_scan_plan.table_name_);
            for (const auto &index_info : table_indexes) {
              BUSTUB_ASSERT(index_info->index_->GetKeySchema()->GetColumnCount() == 1,
                            "key_schema must only has 1 column");
              if (column_name == index_info->index_->GetKeySchema()->GetColumn(0).GetName()) {
                std::vector<AbstractExpressionRef> pred_keys;
                pred_keys.emplace_back((AbstractExpressionRef)expr->GetChildAt(0).get());
                return std::make_shared<IndexScanPlanNode>(seq_scan_plan.output_schema_, seq_scan_plan.table_oid_,
                                                           index_info->index_oid_, seq_scan_plan.filter_predicate_,
                                                           pred_keys);
              }
            }
          }
        }
      }
    }
  }
  return optimized_plan;
  // return plan;
}
}  // namespace bustub
