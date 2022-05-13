//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

/*
bool closed_ = true;
datastore::Filesystem<std::string> root_;
blockstorage::Blockstorage blocks_;
//  pinner::PinManager<Key, Value, Options> pins_;
//  blockstorage::Blockstorage<Key, Value, Options> pinned_block_storage_;
Status OpenRoot();
Status Shurd(const common::Cid& cid);
//  Status OpenLock();
//  Status CloseLock();
 */

#include "repo/repo.hpp"

#include "common/logger/logger.hpp"

namespace cognitio {
namespace repo {

// template <typename StoreValue>
// Repo<StoreValue>::Repo(
//     std::unique_ptr<datastore::Filesystem<StoreValue>> const& root) noexcept
//     : root_(root) {
//   Status mkdir_status = root_->MakeDir("blocks");
//
//   if (!mkdir_status.ok()) {
//     logger_->error("Blocks wasn't created.");
//     return;
//   }
//
//   blockstorage::Blockstorage blocks;
//   Status open_status = blocks.Open(root_->Root() / "blocks");
//
//   if (!open_status.ok()) {
//     logger_->error("Blocks wasn't opened.");
//     return;
//   }
//
//   blocks_ = std::make_unique<blockstorage::Blockstorage>(blocks);
// }

template <typename StoreValue>
void Repo<StoreValue>::initRepoStorage(const std::filesystem::path& path) {
  datastore::Filesystem<StoreValue> root(path);
  root_ = std::make_unique<datastore::Filesystem<StoreValue>>(root);

  blockstorage::Blockstorage blocks(path / "blocks");
  blocks_ = std::make_unique<blockstorage::Blockstorage>(blocks);
}

template <typename StoreValue>
Status Repo<StoreValue>::openRepo() noexcept {
  Status status_open = root_->Open();
  if (!status_open.ok()) {
    return status_open;
  }
  status_open = blocks_->Open();
  return status_open;
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::filesystem::path& path) noexcept {
  initRepoStorage(path);
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::string& name) noexcept {
  std::filesystem::path root = std::filesystem::current_path() / name;
  initRepoStorage(root);
}

template <typename StoreValue>
Status Repo<StoreValue>::Init() noexcept {
  if (!Exists()) {
    initRepoStorage(root_->Root());
  }

  closed_ = false;
}

template <typename StoreValue>
std::string Repo<StoreValue>::shard(const cognitio::common::Cid& cid,
                                    size_t name_length) {
  std::string str_cid = cid.ToString();
  size_t offset = str_cid.length() - name_length - 1;
  return std::string(str_cid.cbegin() + static_cast<long>(offset),
                     str_cid.cend());
}

template <typename StoreValue>
Status Repo<StoreValue>::Add(const common::Cid& cid,
                             const std::vector<uint8_t>& data) noexcept {
  if (!closed_) {
    std::string name_of_shard = shard(cid);

    blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);
    Status is_opened = block.Open();

    if (!is_opened.ok()) {
      return is_opened;
    }

    Status is_added = block.Put(cid, data);

    return is_added;
  }
  return Status(StatusCode::CANCELLED, "Open repo before adding.");
}

template <typename StoreValue>
Status Repo<StoreValue>::Delete(const common::Cid& cid) noexcept {
  if (!closed_) {
    if (!blocks_->Has(cid)) {
      return Status(StatusCode::FAILED,
                    "No such file or directory: " + cid.ToString() + ".");
    }

    std::string name_of_shard = shard(cid);

    blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);

    Status is_opened = block.Open();

    if (!is_opened.ok()) {
      return is_opened;
    }

    Status is_deleted = block.Delete(cid);

    return is_deleted;
  }
  return Status(StatusCode::CANCELLED, "Open repo before deleting.");
}

template <typename StoreValue>
bool Repo<StoreValue>::Exists() noexcept {
  return std::filesystem::exists(root_->Root());
}

}  // namespace repo
}  // namespace cognitio
