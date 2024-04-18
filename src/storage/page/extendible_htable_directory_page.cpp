//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_htable_directory_page.cpp
//
// Identification: src/storage/page/extendible_htable_directory_page.cpp
//
// Copyright (c) 2015-2023, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/page/extendible_htable_directory_page.h"

#include <algorithm>
#include <unordered_map>

#include <deque>
#include "common/config.h"
#include "common/logger.h"
namespace bustub {

void ExtendibleHTableDirectoryPage::Init(uint32_t max_depth) {
  if (max_depth > 2) {
    max_depth = 2;
  }
  max_depth_ = max_depth;
  global_depth_ = 0;
  now_num_ = 1;
  for (uint32_t i = 0; i < HTABLE_DIRECTORY_ARRAY_SIZE; i++) {
    local_depths_[i] = 0;
  }
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}

auto ExtendibleHTableDirectoryPage::HashToBucketIndex(uint32_t hash) const -> uint32_t {
  std::deque<int> binary_hash;
  // uint32_t i = max_depth_;
  while (hash > 0) {
    // 将 n 的最低位与 1 进行按位与操作，得到当前位的值
    int bit = hash & 1;
    // 将当前位的值转换为字符，添加到二进制字符串的开头
    binary_hash.push_front(bit);
    // 将 n 右移一位，继续处理下一位
    hash >>= 1;
  }
  while (binary_hash.size() < 32) {
    binary_hash.push_front(0);
  }
  uint32_t index = 0;
  for (uint32_t j = 0; j < global_depth_; j++) {
    if (binary_hash.back()) {
      index += (1 << j);
    }
    binary_hash.pop_back();
  }
  return index;
  // 该函数已实现
}

auto ExtendibleHTableDirectoryPage::GetBucketPageId(uint32_t bucket_idx) const -> page_id_t {
  return bucket_page_ids_[bucket_idx];
  // 该函数已实现
}

void ExtendibleHTableDirectoryPage::SetBucketPageId(uint32_t bucket_idx, page_id_t bucket_page_id) {
  bucket_page_ids_[bucket_idx] = bucket_page_id;
  //   throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
  // 该函数已实现
}

auto ExtendibleHTableDirectoryPage::GetSplitImageIndex(uint32_t bucket_idx) const -> uint32_t {
  if (bucket_idx < now_num_ / 2) {
    return bucket_idx + now_num_ / 2;
  } else {
    return bucket_idx - now_num_ / 2;
  }
}

auto ExtendibleHTableDirectoryPage::GetGlobalDepth() const -> uint32_t { return global_depth_; }

void ExtendibleHTableDirectoryPage::IncrGlobalDepth() {
  global_depth_++;

  for (uint32_t i = 0; i < now_num_; i++) {
    local_depths_[i + now_num_] = local_depths_[i];
    bucket_page_ids_[i + now_num_] = bucket_page_ids_[i];
  }
  now_num_ = now_num_ << 1;
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}

void ExtendibleHTableDirectoryPage::DecrGlobalDepth() {
  if (global_depth_ > 0) {
    global_depth_--;
    now_num_ = now_num_ >> 1;
  }
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}

auto ExtendibleHTableDirectoryPage::CanShrink() -> bool {
  bool bo = true;
  for (uint32_t i = 0; i < now_num_; i++) {
    if (local_depths_[i] == global_depth_) {
      bo = false;
      break;
    }
  }
  return bo;
}

auto ExtendibleHTableDirectoryPage::Size() const -> uint32_t {
  return now_num_;
  // 该函数已实现
}

auto ExtendibleHTableDirectoryPage::GetLocalDepth(uint32_t bucket_idx) const -> uint32_t {
  u_int32_t local_depth = local_depths_[bucket_idx];
  return local_depth;
}

void ExtendibleHTableDirectoryPage::SetLocalDepth(uint32_t bucket_idx, uint8_t local_depth) {
  local_depths_[bucket_idx] = local_depth;
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}

void ExtendibleHTableDirectoryPage::IncrLocalDepth(uint32_t bucket_idx) {
  local_depths_[bucket_idx]++;
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}

void ExtendibleHTableDirectoryPage::DecrLocalDepth(uint32_t bucket_idx) {
  if (local_depths_[bucket_idx] > 0) {
    local_depths_[bucket_idx]--;
  }
  // throw NotImplementedException("ExtendibleHTableDirectoryPage is not implemented");
}
auto ExtendibleHTableDirectoryPage::GetGlobalDepthMask() const -> uint32_t {
  uint32_t mask = 0;
  for (uint32_t i = 0; i < global_depth_; ++i) {
    mask |= (1 << i);  // 将第 i 位设为 1
  }
  return mask;
}
auto ExtendibleHTableDirectoryPage::GetLocalDepthMask(uint32_t bucket_idx) const -> uint32_t {
  uint32_t local_depth = local_depths_[bucket_idx];

  // 构建局部深度掩码
  uint32_t mask = 0;
  for (uint32_t i = 0; i < local_depth; ++i) {
    mask |= (1 << i);  // 将第 i 位设为 1
  }

  return mask;
}
}  // namespace bustub
