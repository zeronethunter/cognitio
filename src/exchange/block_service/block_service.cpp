#include "exchange/block_service/block_service.hpp"

namespace cognitio {
namespace exchange {
template <typename Value>
Status BlockService<Value>::Open(const std::filesystem::path& path,
                                 bool is_daemon_opened) noexcept {
  Status status_open;
  is_daemon_opened_ = is_daemon_opened;
  if (is_daemon_opened_) {
    status_open = block_swap_->Open();
    if (status_open.ok()) {
      closed_ = false;
    }
    return status_open;
  }
  repo_ =
      std::make_unique<repo::Repo<std::string>>(repo::Repo<std::string>(path));
  status_open = repo_->Init();
  if (status_open.ok()) {
    closed_ = false;
  }
  return status_open;
}

template <typename Value>
Status BlockService<Value>::Close() noexcept {}

template <typename Value>
std::filesystem::path BlockService<Value>::Root() const noexcept {
  if (!is_daemon_opened_) {
    if (repo_->Exists()) {
      return repo_->Root();
    }
    logger_->error("Can't get root. Open repo first.");
    return std::filesystem::path();
  }
  logger_->info("Daemon is opened. Can't get root.");
  return std::filesystem::path();
}

template <typename Value>
Status BlockService<Value>::Put(const common::Cid& key,
                                const Value& value) noexcept {
  return is_daemon_opened_ ? block_swap_->Put(key, value)
                           : repo_->Add(key, value);
  ;
}

template <typename Value>
Value BlockService<Value>::Get(const common::Cid& key) const noexcept {
  return is_daemon_opened_ ? block_swap_->Get(key) : repo_->Get(key);
}

template <typename Value>
Status BlockService<Value>::Delete(const common::Cid& key) noexcept {
  return is_daemon_opened_ ? block_swap_->Delete(key) : repo_->Delete(key);
}

template <typename Value>
bool BlockService<Value>::Has(const common::Cid& key) const noexcept {
  return is_daemon_opened_ ? block_swap_->Has(key) : repo_->Has(key);
}

template <typename Value>
Status BlockService<Value>::PutMany(
    const std::set<std::pair<common::Cid, Value>>& source) noexcept {
  for (auto const& pair : source) {
    Status status_put = Put(pair.first, pair.second);
    if (!status_put.ok()) {
      return status_put;
    }
  }
}

template <typename Value>
std::set<Value> BlockService<Value>::GetMany(
    const std::set<common::Cid>& source) const noexcept {
  for (auto const& key : source) {
    Value value = Get(key);
  }
}

template <typename Value>
Status BlockService<Value>::DeleteMany(
    const std::set<common::Cid>& source) noexcept {
  for (auto const& key : source) {
    Status status_put = Delete(key);
    if (!status_put.ok()) {
      return status_put;
    }
  }
}
}  // namespace exchange
}  // namespace cognitio
