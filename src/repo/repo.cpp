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

template <typename StoreValue>
Repo<StoreValue>::Repo(
    std::unique_ptr<datastore::Filesystem<StoreValue>> const& root) noexcept
    : root_(std::move(root)), closed_(false) {}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::string& path) noexcept
    : root_(std::move(std::make_unique<datastore::Filesystem<StoreValue>>(
          datastore::Filesystem<StoreValue>(std::filesystem::path(path))))) {}

template <typename StoreValue>
Status Repo<StoreValue>::Init(const std::string& path) noexcept {
  std::filesystem::path root(path);

  datastore::Filesystem<StoreValue> root_storage;
  blockstorage::Blockstorage blocks;

  Status open_root = root_storage.Open(root);

  root_storage.MakeDir("blocks");
  Status open_blocks = blocks.Open(root / "blocks");

  if (!open_root.ok() && !open_blocks.ok()) {
    logger_->error("Unable to open {}",
                   !open_root.ok() ? " root storage." : " block storage.");
  }

  root_ = std::make_unique<datastore::Filesystem<StoreValue>>(root_storage);
  blocks_ = std::make_unique<blockstorage::Blockstorage>(blocks);
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

    blockstorage::Blockstorage block;
    Status is_opened = block.Open(blocks_->Root() / name_of_shard);

    if (!is_opened.ok()) {
      return is_opened;
    }

    Status is_added = block.Put(cid, data);

    if (!is_added.ok()) {
      return is_added;
    }

    return Status::OK;
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

    blockstorage::Blockstorage block;

    Status is_opened = block.Open(blocks_->Root() / name_of_shard);

    if (!is_opened.ok()) {
      return is_opened;
    }

    Status is_deleted = block.Delete(cid);

    if (!is_deleted.ok()) {
      return is_deleted;
    }

    return Status::OK;
  }
  return Status(StatusCode::CANCELLED, "Open repo before deleting.");
}

template <typename StoreValue>
Status Repo<StoreValue>::openRepo() noexcept {}

}  // namespace repo
}  // namespace cognitio
