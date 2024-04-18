//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// disk_extendible_hash_table.cpp
//
// Identification: src/container/disk/hash/disk_extendible_hash_table.cpp
//
// Copyright (c) 2015-2023, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common/config.h"
#include "common/exception.h"
#include "common/logger.h"
#include "common/macros.h"
#include "common/rid.h"
#include "common/util/hash_util.h"
#include "container/disk/hash/disk_extendible_hash_table.h"
#include "storage/index/hash_comparator.h"
#include "storage/page/extendible_htable_bucket_page.h"
#include "storage/page/extendible_htable_directory_page.h"
#include "storage/page/extendible_htable_header_page.h"
#include "storage/page/page_guard.h"

namespace bustub {

template <typename K, typename V, typename KC>
DiskExtendibleHashTable<K, V, KC>::DiskExtendibleHashTable(const std::string &name, BufferPoolManager *bpm,
                                                           const KC &cmp, const HashFunction<K> &hash_fn,
                                                           uint32_t header_max_depth, uint32_t directory_max_depth,
                                                           uint32_t bucket_max_size)
    : bpm_(bpm),
      cmp_(cmp),
      hash_fn_(std::move(hash_fn)),
      header_max_depth_(header_max_depth),
      directory_max_depth_(directory_max_depth),
      bucket_max_size_(bucket_max_size) {
  BasicPageGuard header_guard = bpm->NewPageGuarded(&header_page_id_);
  auto header = header_guard.AsMut<ExtendibleHTableHeaderPage>();
  header->Init(header_max_depth_);
}

/*****************************************************************************
 * SEARCH
 *****************************************************************************/
template <typename K, typename V, typename KC>
auto DiskExtendibleHashTable<K, V, KC>::GetValue(const K &key, std::vector<V> *result, Transaction *transaction) const
    -> bool {
  // transaction->Begin();
  BasicPageGuard header_guard = bpm_->FetchPageBasic(header_page_id_);
  auto header = header_guard.AsMut<ExtendibleHTableHeaderPage>();
  uint32_t hash = Hash(key);
  uint32_t directory_idx = header->HashToDirectoryIndex(hash);
  page_id_t directory_page_id = header->GetDirectoryPageId(directory_idx);
  BasicPageGuard directory_guard = bpm_->FetchPageBasic(directory_page_id);
  auto directory = directory_guard.AsMut<ExtendibleHTableDirectoryPage>();

  uint32_t bucket_idx = directory->HashToBucketIndex(hash);
  page_id_t bucket_page_id = directory->GetBucketPageId(bucket_idx);
  BasicPageGuard bucket_guard = bpm_->FetchPageBasic(bucket_page_id);
  auto bucket = bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
  V value;
  if (bucket->Lookup(key, value, cmp_)) {
    result->push_back(value);
    return true;
    // transaction->Commit();
  }
  return false;
}

/*****************************************************************************
 * INSERTION
 *****************************************************************************/

template <typename K, typename V, typename KC>
auto DiskExtendibleHashTable<K, V, KC>::Insert(const K &key, const V &value, Transaction *transaction) -> bool {
  // transaction->Begin();
  BasicPageGuard header_guard = bpm_->FetchPageBasic(header_page_id_);
  auto header = header_guard.AsMut<ExtendibleHTableHeaderPage>();
  uint32_t hash = Hash(key);
  uint32_t directory_idx = header->HashToDirectoryIndex(hash);

  // 是否要创建新的directory页面
  if (InsertToNewDirectory(header, directory_idx, hash, key, value)) {
    page_id_t directory_page_id = header->GetDirectoryPageId(directory_idx);
    BasicPageGuard directory_guard = bpm_->FetchPageBasic(directory_page_id);
    auto directory_temp = directory_guard.AsMut<ExtendibleHTableDirectoryPage>();

    directory_temp->Init(directory_max_depth_);
  }
  page_id_t directory_page_id = header->GetDirectoryPageId(directory_idx);
  BasicPageGuard directory_guard = bpm_->FetchPageBasic(directory_page_id);
  auto directory = directory_guard.AsMut<ExtendibleHTableDirectoryPage>();
  uint32_t bucket_idx = directory->HashToBucketIndex(hash);
  // 是否要创建新的bucket页面
  if (InsertToNewBucket(directory, bucket_idx, key, value)) {
    page_id_t bucket_page_id = directory->GetBucketPageId(bucket_idx);
    BasicPageGuard bucket_guard = bpm_->FetchPageBasic(bucket_page_id);
    auto bucket_temp = bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
    bucket_temp->Init(bucket_max_size_);
  }

  page_id_t bucket_page_id = directory->GetBucketPageId(bucket_idx);
  BasicPageGuard bucket_guard = bpm_->FetchPageBasic(bucket_page_id);
  auto bucket = bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();

  // 若bucket未满则直接插入
  if (!bucket->IsFull()) {
    if (bucket->Insert(key, value, cmp_)) {
      return true;
    } else {
      return false;
    }
    // transaction->Commit();

  } else {
    if (directory->GetGlobalDepth() < directory_max_depth_) {
      directory->IncrGlobalDepth();
    }
    if (directory->GetGlobalDepth() == directory_max_depth_ &&
        directory->GetLocalDepth(bucket_idx) == directory_max_depth_) {
      return false;
    }
    uint32_t local_depth = directory->GetLocalDepth(bucket_idx);
    // 创建两个新的桶
    page_id_t page_id1;
    page_id_t page_id2;
    BasicPageGuard bucket_guard1 = bpm_->NewPageGuarded(&page_id1);
    auto new_bucket1 = bucket_guard1.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
    BasicPageGuard bucket_guard2 = bpm_->NewPageGuarded(&page_id2);
    auto new_bucket2 = bucket_guard2.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
    new_bucket1->Init(bucket_max_size_);
    new_bucket2->Init(bucket_max_size_);
    uint32_t new_bucket_idx1 = bucket_idx;
    uint32_t new_bucket_idx2;
    // 遍历原桶中的每个元素
    for (uint32_t i = 0; i < bucket->Size(); i++) {
      auto entry = bucket->EntryAt(i);
      uint32_t hash = Hash(entry.first);
      // 根据新局部深度计算元素应该放置在哪个新桶中
      uint32_t new_bucket_idx = hash & ((1 << (local_depth + 1)) - 1);
      // 将元素插入到相应的新桶中
      if (new_bucket_idx == bucket_idx) {
        new_bucket1->Insert(entry.first, entry.second, cmp_);
      } else {
        new_bucket2->Insert(entry.first, entry.second, cmp_);
        new_bucket_idx2 = new_bucket_idx;
      }
    }
    bpm_->DeletePage(bucket_page_id);
    UpdateDirectoryMapping(directory, new_bucket_idx1, page_id1, local_depth + 1, directory->GetLocalDepth(bucket_idx));
    UpdateDirectoryMapping(directory, new_bucket_idx2, page_id2, local_depth + 1, directory->GetLocalDepth(bucket_idx));

    directory_idx = header->HashToDirectoryIndex(hash);
    bucket_idx = directory->HashToBucketIndex(hash);
    page_id_t new_bucket_page_id = directory->GetBucketPageId(bucket_idx);
    BasicPageGuard new_bucket_guard = bpm_->FetchPageBasic(new_bucket_page_id);
    auto new_bucket = new_bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();

    if (new_bucket->Insert(key, value, cmp_)) {
      return true;
    } else {
      return false;
    }
  }
}

template <typename K, typename V, typename KC>
auto DiskExtendibleHashTable<K, V, KC>::InsertToNewDirectory(ExtendibleHTableHeaderPage *header_,
                                                             uint32_t directory_idx, uint32_t hash, const K &key,
                                                             const V &value) -> bool {
  page_id_t directory_page_id = header_->GetDirectoryPageId(directory_idx);
  // BasicPageGuard directory_guard = bpm_->FetchPageBasic(directory_page_id);
  // auto directory = directory_guard.AsMut<ExtendibleHTableDirectoryPage>();
  if (directory_page_id == HEADER_PAGE_ID) {
    page_id_t page_id;
    bpm_->NewPageGuarded(&page_id);
    header_->SetDirectoryPageId(directory_idx, page_id);
    return true;
  } else {
    return false;
  }
}

template <typename K, typename V, typename KC>
auto DiskExtendibleHashTable<K, V, KC>::InsertToNewBucket(ExtendibleHTableDirectoryPage *directory, uint32_t bucket_idx,
                                                          const K &key, const V &value) -> bool {
  page_id_t bucket_page_id = directory->GetBucketPageId(bucket_idx);
  // BasicPageGuard bucket_guard = bpm_->FetchPageBasic(bucket_page_id);
  // auto bucket = bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
  if (bucket_page_id == header_page_id_) {
    page_id_t page_id;
    bpm_->NewPageGuarded(&page_id);
    directory->SetBucketPageId(bucket_idx, page_id);
    return true;
  } else {
    return false;
  }
}

template <typename K, typename V, typename KC>
void DiskExtendibleHashTable<K, V, KC>::UpdateDirectoryMapping(ExtendibleHTableDirectoryPage *directory,
                                                               uint32_t new_bucket_idx, page_id_t new_bucket_page_id,
                                                               uint32_t new_local_depth, uint32_t local_depth_mask) {
  directory->SetBucketPageId(new_bucket_idx, new_bucket_page_id);
  directory->SetLocalDepth(new_bucket_idx, new_local_depth);

  // throw NotImplementedException("DiskExtendibleHashTable is not implemented");
}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
template <typename K, typename V, typename KC>
auto DiskExtendibleHashTable<K, V, KC>::Remove(const K &key, Transaction *transaction) -> bool {
  BasicPageGuard header_guard = bpm_->FetchPageBasic(header_page_id_);
  auto header = header_guard.AsMut<ExtendibleHTableHeaderPage>();

  uint32_t hash = Hash(key);
  uint32_t directory_idx = header->HashToDirectoryIndex(hash);
  page_id_t directory_page_id = header->GetDirectoryPageId(directory_idx);
  BasicPageGuard directory_guard = bpm_->FetchPageBasic(directory_page_id);
  auto directory = directory_guard.AsMut<ExtendibleHTableDirectoryPage>();

  uint32_t bucket_idx = directory->HashToBucketIndex(hash);
  page_id_t bucket_page_id = directory->GetBucketPageId(bucket_idx);
  BasicPageGuard bucket_guard = bpm_->FetchPageBasic(bucket_page_id);
  auto bucket = bucket_guard.AsMut<ExtendibleHTableBucketPage<K, V, KC>>();
  if (bucket->Remove(key, cmp_)) {
    return true;
  } else {
    return false;
  }
}
// template <typename K, typename V, typename KC>
// void DiskExtendibleHashTable<K, V, KC>::MigrateEntries(ExtendibleHTableBucketPage<K, V, KC> *old_bucket,
//                                                        ExtendibleHTableBucketPage<K, V, KC> *new_bucket,
//                                                        uint32_t new_bucket_idx, uint32_t local_depth_mask) {
//   // 如果新存储桶不为空，或者新存储桶的拆分映像与旧存储桶的拆分映像不同，则无法合并
//   if (!new_bucket->IsEmpty()) {
//     return;
//   }

//   // 如果新存储桶的拆分映像为空，则应继续以递归方式合并
//   if (new_bucket->GetSplitImage() == nullptr) {
//     uint32_t new_bucket_global_idx = new_bucket_idx & local_depth_mask;
//     uint32_t old_bucket_global_idx = old_bucket->GetBucketIdx() & local_depth_mask;

//     // 计算合并后的存储桶的新拆分映像
//     uint32_t merged_global_idx = new_bucket_global_idx | (1 << (new_bucket->GetLocalDepth() - 1));
//     uint32_t merged_bucket_idx = merged_global_idx & local_depth_mask;

//     // 获取新合并存储桶的拆分映像存储桶
//     ExtendibleHTableBucketPage<K, V, KC> *merged_bucket = directory_[merged_bucket_idx];

//     // 递归调用合并函数
//     MigrateEntries(old_bucket, merged_bucket, merged_bucket_idx, local_depth_mask);
//   }

//   // 合并完毕后，将新存储桶的拆分映像指向合并后的存储桶
//   new_bucket->SetSplitImage(new_bucket->GetSplitImage());
// }

template class DiskExtendibleHashTable<int, int, IntComparator>;
template class DiskExtendibleHashTable<GenericKey<4>, RID, GenericComparator<4>>;
template class DiskExtendibleHashTable<GenericKey<8>, RID, GenericComparator<8>>;
template class DiskExtendibleHashTable<GenericKey<16>, RID, GenericComparator<16>>;
template class DiskExtendibleHashTable<GenericKey<32>, RID, GenericComparator<32>>;
template class DiskExtendibleHashTable<GenericKey<64>, RID, GenericComparator<64>>;
}  // namespace bustub
