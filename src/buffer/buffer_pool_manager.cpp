//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// buffer_pool_manager.cpp
//
// Identification: src/buffer/buffer_pool_manager.cpp
//
// Copyright (c) 2015-2021, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/buffer_pool_manager.h"

#include "common/exception.h"
#include "common/macros.h"
#include "storage/page/page_guard.h"

namespace bustub {

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager, size_t replacer_k,
                                     LogManager *log_manager)
    : pool_size_(pool_size), disk_scheduler_(std::make_unique<DiskScheduler>(disk_manager)), log_manager_(log_manager) {
  // TODO(students): remove this line after you have implemented the buffer pool manager
  // throw NotImplementedException(
  //     "BufferPoolManager is not implemented yet. If you have finished implementing BPM, please remove the throw "
  //     "exception line in `buffer_pool_manager.cpp`.");

  // we allocate a consecutive memory space for the buffer pool
  pages_ = new Page[pool_size_];
  replacer_ = std::make_unique<LRUKReplacer>(pool_size, replacer_k);

  // Initially, every page is in the free list.
  for (size_t i = 0; i < pool_size_; ++i) {
    free_list_.emplace_back(static_cast<int>(i));
  }
}

BufferPoolManager::~BufferPoolManager() { delete[] pages_; }

auto BufferPoolManager::NewPage(page_id_t *page_id) -> Page * {
  std::unique_lock<std::mutex> lock(latch_);
  if (free_list_.empty() && replacer_.get()->Size() == 0) {
    // lock.unlock();
    return nullptr;
  }
  frame_id_t temp = 0;
  if (!free_list_.empty()) {
    temp = free_list_.front();
    free_list_.pop_front();
  } else {
    replacer_.get()->Evict(&temp);
    if (pages_[temp].IsDirty()) {
      FlushPage(pages_[temp].GetPageId());
    }
    page_table_.erase(pages_[temp].GetPageId());
  }
  *page_id = AllocatePage();

  page_table_[*page_id] = temp;

  pages_[temp].ResetMemory();
  pages_[temp].SetPageId(*page_id);
  pages_[temp].SetDirty(false);
  pages_[temp].pin_count_ = 1;
  replacer_.get()->RecordAccess(temp);
  replacer_.get()->SetEvictable(temp, false);

  // lock.unlock();
  return &pages_[temp];
}

auto BufferPoolManager::FetchPage(page_id_t page_id, [[maybe_unused]] AccessType access_type) -> Page * {
  std::unique_lock<std::mutex> lock(latch_);
  if (page_table_.find(page_id) == page_table_.end()) {
    if (free_list_.empty() && replacer_.get()->Size() == 0) {
      // lock.unlock();
      return nullptr;
    }
    frame_id_t temp = 0;
    if (!free_list_.empty()) {
      temp = free_list_.front();
      free_list_.pop_front();
    } else {
      replacer_.get()->Evict(&temp);
      if (pages_[temp].IsDirty()) {
        FlushPage(pages_[temp].GetPageId());
      }
      page_table_.erase(pages_[temp].GetPageId());
    }
    replacer_.get()->RecordAccess(temp);
    replacer_.get()->SetEvictable(temp, false);

    page_table_[page_id] = temp;

    auto promise = disk_scheduler_.get()->CreatePromise();
    auto future = promise.get_future();
    DiskRequest tempRequest{false, pages_[temp].GetData(), page_id, std::move(promise)};
    disk_scheduler_.get()->Schedule(std::move(tempRequest));
    future.get();
    pages_[temp].SetPageId(page_id);
    pages_[temp].SetDirty(false);
    pages_[temp].pin_count_ = 1;
    // lock.unlock();
    return &pages_[temp];
  } else {
    auto it = page_table_.find(page_id);
    frame_id_t temp = it->second;
    pages_[temp].pin_count_++;
    pages_[temp].SetDirty(true);
    replacer_.get()->RecordAccess(temp);
    replacer_.get()->SetEvictable(temp, false);
    // lock.unlock();
    return &pages_[temp];
  }
}

auto BufferPoolManager::UnpinPage(page_id_t page_id, bool is_dirty, [[maybe_unused]] AccessType access_type) -> bool {
  std::unique_lock<std::mutex> lock(latch_);
  if (page_table_.find(page_id) == page_table_.end()) {
    // lock.unlock();
    return false;
  }
  auto it = page_table_.find(page_id);
  frame_id_t temp = it->second;
  if (pages_[temp].GetPinCount() == 0) {
    // lock.unlock();
    return false;
  }
  pages_[temp].pin_count_--;
  if ((pages_[temp].GetPinCount() == 0)) {
    replacer_.get()->SetEvictable(temp, true);
  }
  pages_[temp].SetDirty(is_dirty);
  // lock.unlock();
  return true;
}

auto BufferPoolManager::FlushPage(page_id_t page_id) -> bool {
  if (page_table_.find(page_id) == page_table_.end()) {
    // lock.unlock();
    return false;
  }
  auto it = page_table_.find(page_id);
  frame_id_t temp = it->second;

  DiskRequest tempRequest;
  tempRequest.is_write_ = true;
  tempRequest.data_ = pages_[temp].GetData();
  tempRequest.page_id_ = pages_[temp].GetPageId();
  auto promise = disk_scheduler_->CreatePromise();
  auto future = promise.get_future();
  tempRequest.callback_ = std::move(promise);
  disk_scheduler_.get()->Schedule(std::move(tempRequest));
  future.get();

  // lock.unlock();
  return true;
}

void BufferPoolManager::FlushAllPages() {
  for (size_t i = 0; i < pool_size_; i++) {
    FlushPage(pages_[i].page_id_);
  }
}

auto BufferPoolManager::DeletePage(page_id_t page_id) -> bool {
  std::unique_lock<std::mutex> lock(latch_);
  if (page_table_.find(page_id) == page_table_.end()) {
    // lock.unlock();
    return true;
  }
  auto it = page_table_.find(page_id);
  frame_id_t temp = it->second;
  if (pages_[temp].GetPinCount() > 0) {
    // lock.unlock();
    return false;
  }
  if (pages_[temp].IsDirty()) {
    FlushPage(page_id);
  }
  page_table_.erase(pages_[temp].GetPageId());
  pages_[temp].ResetMemory();
  pages_[temp].ResetPageId();
  pages_[temp].ResetDirty();

  replacer_.get()->Remove(temp);
  free_list_.push_back(temp);
  DeallocatePage(page_id);
  lock.unlock();
  return true;
}

auto BufferPoolManager::AllocatePage() -> page_id_t { return next_page_id_++; }

auto BufferPoolManager::FetchPageBasic(page_id_t page_id) -> BasicPageGuard {
  Page *page = FetchPage(page_id);
  return {this, page};
}

auto BufferPoolManager::FetchPageRead(page_id_t page_id) -> ReadPageGuard {
  Page *page = FetchPage(page_id);
  return {this, page};
}

auto BufferPoolManager::FetchPageWrite(page_id_t page_id) -> WritePageGuard {
  Page *page = FetchPage(page_id);
  return {this, page};
}

auto BufferPoolManager::NewPageGuarded(page_id_t *page_id) -> BasicPageGuard {
  Page *page = NewPage(page_id);
  return {this, page};
}

}  // namespace bustub
