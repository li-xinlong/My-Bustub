//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include "common/exception.h"

namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}

auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool {
  std::unique_lock<std::mutex> lock(latch_);
  bool b = false;
  size_t min_timestamp = std::numeric_limits<size_t>::max();  // 初始设置为最大值
  frame_id_t evict_id = 0;
  bool b1 = 0;
  // 遍历节点，选择驱逐帧
  for (auto it = node_store_.begin(); it != node_store_.end(); ++it) {
    if (it->second.is_evictable_) {
      if (it->second.history_.size() < k_) {
        b1 = 1;
        break;
      } else {
        size_t last_access_time = it->second.history_.back();
        if (last_access_time <= min_timestamp) {
          min_timestamp = last_access_time;
          evict_id = it->first;
          b = true;
        }
      }
    }
  }
  if (b1 == 1) {
    min_timestamp = std::numeric_limits<size_t>::max();
    for (auto it = node_store_.begin(); it != node_store_.end(); ++it) {
      if (it->second.is_evictable_) {
        if (it->second.history_.size() < k_) {
          size_t last_access_time = it->second.history_.back();
          if (last_access_time <= min_timestamp) {
            min_timestamp = last_access_time;
            evict_id = it->first;
            b = true;
          }
        }
      }
    }
  }
  if (b) {
    node_store_.erase(evict_id);
    *frame_id = evict_id;
    curr_size_--;
  }

  lock.unlock();
  return b;
}

void LRUKReplacer::RecordAccess(frame_id_t frame_id, [[maybe_unused]] AccessType access_type) {
  std::unique_lock<std::mutex> lock(latch_);
  if (static_cast<size_t>(frame_id) > replacer_size_) {
    throw std::invalid_argument("Invalid frame ID: frame ID exceeds the replacer size");
  }
  // 如果历史队列中没有则
  if (node_store_.find(frame_id) == node_store_.end()) {
    LRUKNode newNode(k_, frame_id);
    newNode.history_.push_front(current_timestamp_++);
    node_store_.insert(std::make_pair(frame_id, newNode));
    // node_store_[frame_id].history_.push_front(current_timestamp_);
  } else {
    LRUKNode &temp = node_store_.at(frame_id);
    temp.history_.push_front(current_timestamp_++);
    if (temp.history_.size() >= k_ + 1) {
      while (temp.history_.size() > k_) {
        temp.history_.pop_back();
      }
    }
  }
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
  std::unique_lock<std::mutex> lock(latch_);
  if (set_evictable == false) {
    if (node_store_.at(frame_id).is_evictable_ == true) {
      curr_size_--;
    }
    node_store_.at(frame_id).is_evictable_ = false;
  } else {
    if (node_store_.at(frame_id).is_evictable_ == false) {
      curr_size_++;
    }
    node_store_.at(frame_id).is_evictable_ = true;
  }
  lock.unlock();
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
  std::unique_lock<std::mutex> lock(latch_);
  if (node_store_.find(frame_id) == node_store_.end()) {
    return;
  } else {
    if (node_store_.at(frame_id).is_evictable_ == false) {
      throw Exception("LRUKNode is not evitable!");
    } else {
      node_store_.erase(frame_id);
      curr_size_--;
    }
  }
}

auto LRUKReplacer::Size() -> size_t { return curr_size_; }

}  // namespace bustub
