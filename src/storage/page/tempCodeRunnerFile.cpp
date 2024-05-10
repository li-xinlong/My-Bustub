
BasicPageGuard::BasicPageGuard(BasicPageGuard &&that) noexcept {
  this->bpm_ = std::move(that.bpm_);
  this->page_ = std::move(that.page_);
  this->is_dirty_ = that.is_dirty_;
  that.bpm_ = nullptr;
  that.page_ = nullptr;
  that.is_dirty_ = false;
}

void BasicPageGuard::Drop() {
  if (page_) {
    bpm_->UnpinPage(page_->GetPageId(), is_dirty_);
  }
  bpm_ = nullptr;
  page_ = nullptr;
  is_dirty_ = false;
}

auto BasicPageGuard::operator=(BasicPageGuard &&that) noexcept -> BasicPageGuard & {
  Drop();
  this->bpm_ = std::move(that.bpm_);
  this->page_ = std::move(that.page_);
  this->is_dirty_ = that.is_dirty_;
  return *this;
}

BasicPageGuard::~BasicPageGuard() { Drop(); };  // NOLINT

auto BasicPageGuard::UpgradeRead() -> ReadPageGuard {
  ReadPageGuard temp(bpm_, page_);
  Drop();
  return temp;
}

auto BasicPageGuard::UpgradeWrite() -> WritePageGuard {
  WritePageGuard temp(bpm_, page_);
  Drop();
  return temp;
}

ReadPageGuard::ReadPageGuard(BufferPoolManager *bpm, Page *page) { guard_ = BasicPageGuard(bpm, page); }

ReadPageGuard::ReadPageGuard(ReadPageGuard &&that) noexcept = default;

auto ReadPageGuard::operator=(ReadPageGuard &&that) noexcept -> ReadPageGuard & {
  guard_ = std::move(that.guard_);
  return *this;
}

void ReadPageGuard::Drop() { guard_.Drop(); }

ReadPageGuard::~ReadPageGuard() { guard_.~BasicPageGuard(); }  // NOLINT

WritePageGuard::WritePageGuard(BufferPoolManager *bpm, Page *page) { guard_ = BasicPageGuard(bpm, page); }

WritePageGuard::WritePageGuard(WritePageGuard &&that) noexcept = default;

auto WritePageGuard::operator=(WritePageGuard &&that) noexcept -> WritePageGuard & {
  guard_ = std::move(that.guard_);
  return *this;
}

void WritePageGuard::Drop() { guard_.Drop(); }

WritePageGuard::~WritePageGuard() { guard_.~BasicPageGuard(); }  // NOLINT
