//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_htable_header_page.cpp
//
// Identification: src/storage/page/extendible_htable_header_page.cpp
//
// Copyright (c) 2015-2023, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/page/extendible_htable_header_page.h"

#include <deque>
#include "common/exception.h"
namespace bustub {

void ExtendibleHTableHeaderPage::Init(uint32_t max_depth) {
  if (max_depth > 2) {
    max_depth = 2;
  }
  max_depth_ = max_depth;
  // throw NotImplementedException("ExtendibleHTableHeaderPage is not implemented");
}

auto ExtendibleHTableHeaderPage::HashToDirectoryIndex(uint32_t hash) const -> uint32_t {
  std::deque<int> binary_hash;
  // uint32_t i = max_depth_;
  while (hash > 0) {
    // 将 n 的最低位与 1 进行按位与操作，得到当前位的值
    int bit = hash & 1;
    // 将当前位的值转换为字符，添加到二进制字符串的开头
    binary_hash.push_back(bit);
    // 将 n 右移一位，继续处理下一位
    hash >>= 1;
  }
  while (binary_hash.size() < 32) {
    binary_hash.push_back(0);
  }
  uint32_t index = 0;
  for (int j = max_depth_ - 1; j >= 0; j--) {
    if (binary_hash.back()) {
      index += (1 << j);
    }
    binary_hash.pop_back();
  }

  return index;
}

auto ExtendibleHTableHeaderPage::GetDirectoryPageId(uint32_t directory_idx) const -> uint32_t {
  return directory_page_ids_[directory_idx];
}

void ExtendibleHTableHeaderPage::SetDirectoryPageId(uint32_t directory_idx, page_id_t directory_page_id) {
  directory_page_ids_[directory_idx] = directory_page_id;
  // throw NotImplementedException("ExtendibleHTableHeaderPage is not implemented");
}

auto ExtendibleHTableHeaderPage::MaxSize() const -> uint32_t { return max_depth_; }

}  // namespace bustub
