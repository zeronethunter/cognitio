//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/repo.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <memory>
#include <mutex>
#include <sstream>
#include <type_traits>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "config/config.hpp"

namespace cognitio {
namespace repo {

template <typename StoreValue>
void Repo<StoreValue>::initRepoStorage(
    const std::filesystem::path& path) noexcept {
  datastore::Filesystem<StoreValue> root(path);
  root_ = std::make_unique<datastore::Filesystem<StoreValue>>(root);

  blockstorage::Blockstorage blocks(path / "blocks");
  blocks_ = std::make_unique<blockstorage::Blockstorage>(std::move(blocks));

  pinner_ = std::make_unique<pinner::PinManager>(path);

  if (Exists()) {
    closed_ = false;
  }
}

template <typename StoreValue>
Status Repo<StoreValue>::openRepo() noexcept {
  logger_->debug("Opening repo.");

  Status status_open = root_->Open();
  if (!status_open.ok()) {
    return status_open;
  }
  status_open = blocks_->Open();

  if (!status_open.ok()) {
    return status_open;
  }

  pinner_->Dump();

  Status status_init = config_.Dump();

  return status_init;
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::filesystem::path& path) noexcept
    : config_(config::Config::GetInstance()) {
  initRepoStorage(path);
  logger_ = common::logger::createLogger("repo");
  is_running_ = false;
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::string& name) noexcept
    : config_(config::Config::GetInstance()) {
  initRepoStorage(std::filesystem::path(name));
  logger_ = common::logger::createLogger("repo");
  is_running_ = false;
}

template <typename StoreValue>
Status Repo<StoreValue>::Init() noexcept {
  Status err = Status::OK;
  if (!root_) {
    logger_->error("I don't know where to init repository.");
    return Status::FAILED;
  }
  logger_->debug("Repo initializing");

  if (!Exists()) {
    logger_->info("Repo not found. Creating one.");
    initRepoStorage(root_->Root());
    err = openRepo();
    if (err.ok()) {
      logger_->info("Successfully created here: {}", root_->Root().string());
    }
  } else {
    logger_->info("Repo was found in here: {}", root_->Root().string());
  }

  closed_ = false;
  return err;
}

template <typename StoreValue>
std::string Repo<StoreValue>::shard(const common::Cid& cid,
                                    size_t name_length) const noexcept {
  std::string str_cid = cid.ToString();
  size_t offset = str_cid.length() - name_length;
  return {str_cid.cbegin() + static_cast<long>(offset), str_cid.cend()};
}

template <typename StoreValue>
Status Repo<StoreValue>::Add(const ProtoBlock& block, bool is_pinned) noexcept {
  if (is_pinned) {
    pinner_->Pin(block.GetCid());
  }
  logger_->debug("Adding block to repo.");
  std::unique_ptr<Block> proto_block = block.ToProtoMessage();

  std::stringstream dump(std::ios_base::out | std::ios_base::trunc |
                         std::ios_base::binary);

  proto_block->SerializeToOstream(&dump);
  std::string dump_str = dump.str();
  if (dump_str.empty()) {
    logger_->warn("Block is empty.");
  }

  Status status = addByKey(block.GetCid(), {dump_str.begin(), dump_str.end()});

  if (status.ok()) {
    logger_->info("Successfully added {}", block.GetCid().ToString());
    return Status::OK;
  }
  if (status.error_code() == StatusCode::ALREADY_EXISTS) {
    logger_->info("Already exists {}", block.GetCid().ToString());
    return Status::OK;
  }

  cv_.notify_all();
  return status;
}

template <typename StoreValue>
linked_data::ProtoBlock Repo<StoreValue>::Get(
    const common::Cid& key) const noexcept {
  if (!closed_) {
    logger_->debug("Getting block from repo.");
    std::vector<uint8_t> content = getByKey(key);

    std::unique_ptr<Block> proto_block = std::make_unique<Block>();
    std::stringstream data(std::string(content.begin(), content.end()),
                           std::ios_base::in | std::ios_base::binary);

    if (!proto_block->ParseFromIstream(&data)) {
      logger_->debug("Can't find block in repo -> going to blockswap.");
    }

    if (!proto_block->IsInitialized()) {
      logger_->warn("Block is not initialized.");
    }

    ProtoBlock result;
    result.FromProtoMessage(proto_block);

    logger_->debug("Successfully get block.");
    return result;
  }

  logger_->error("Open repo before getting.");
  return {};
}

template <typename StoreValue>
Status Repo<StoreValue>::Reset() {
  if (!std::filesystem::exists(root_->Root())) {
    return {StatusCode::NOT_FOUND, "Repo not fount, nothing to reset."};
  }
  if (!std::filesystem::exists(blocks_->Root())) {
    return {StatusCode::NOT_FOUND, "Blocks not found, nothing to reset."};
  }
  if (!config_.IsLocal()) logger_->debug("Deleting blocks...");
  std::vector<std::filesystem::path> paths_to_delete;
  for (auto const& element :
       std::filesystem::recursive_directory_iterator(blocks_->Root())) {
    paths_to_delete.push_back(element.path());
  }
  for (auto const& path : paths_to_delete) {
    if (!is_directory(path)) {
      std::filesystem::remove(path);
    }
  }
  for (auto const& path : paths_to_delete) {
    if (is_directory(path)) {
      std::filesystem::remove(path);
    }
  }
  logger_->debug("Clearing config...");

  config_.GetInstance().Reset();
  Status status = config_.Dump();

  if (!status.ok()) {
    logger_->info("Failed to reset config.");
  }

  if (pinner_->Exists()) {
    logger_->debug("Clearing pins...");
    std::fstream pins(root_->Root() / "pins", std::ios::out | std::ios::trunc);
    pins.close();
  }
  return Status::OK;
}

template <typename StoreValue>
Status Repo<StoreValue>::addByKey(const common::Cid& cid,
                                  const std::vector<uint8_t>& data) noexcept {
  std::string name_of_shard = shard(cid);

  blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);
  Status is_opened = block.Open();

  if (!is_opened.ok()) {
    return is_opened;
  }

  if (block.Has(cid)) {
    logger_->info("Block is already exists at {}",
                  (block.Root() / cid.ToString()).string());
    return {StatusCode::ALREADY_EXISTS};
  }

  logger_->info("Adding block with cid: {}", cid.ToString());
  Status is_added = block.Put(cid, data);

  if (is_added.ok()) {
    logger_->debug("Successfully added cid: {}", cid.ToString());
  }

  return is_added;
}

template <typename StoreValue>
Status Repo<StoreValue>::deleteByKey(const common::Cid& cid) noexcept {
  if (!closed_) {
    std::string name_of_shard = shard(cid);

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard /
                                 cid.ToString())) {
      return {StatusCode::FAILED,
              "No such file or directory: " + cid.ToString() + "."};
    }

    blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);

    Status is_opened = block.Open();

    if (!is_opened.ok()) {
      return is_opened;
    }

    logger_->info("Deleting block with cid: {}", cid.ToString());
    Status is_deleted = block.Delete(cid);

    if (is_deleted.ok()) {
      logger_->debug("Successfully deleted cid: {}", cid.ToString());
    }

    return is_deleted;
  }
  return {StatusCode::CANCELLED, "Open repo before deleting."};
}

template <typename StoreValue>
std::vector<uint8_t> Repo<StoreValue>::getByKey(
    const common::Cid& cid) const noexcept {
  if (!closed_) {
    std::string name_of_shard = shard(cid);

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard /
                                 cid.ToString())) {
      logger_->error("Can't get value by cid. It doesn't exist.");
      return {};
    }

    blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);

    logger_->info("Getting block with cid: {}", cid.ToString());
    std::pair<Status, std::vector<uint8_t>> result = block.Get(cid);

    if (!result.first.ok()) {
      logger_->error(result.first.error_message());
      return {};
    }

    return result.second;
  }

  logger_->error("Repo is closed");
  return {};
}

template <typename StoreValue>
bool Repo<StoreValue>::Has(const common::Cid& cid) const noexcept {
  if (!closed_) {
    std::string name_of_shard = shard(cid);

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard /
                                 cid.ToString())) {
      return false;
    }

    blockstorage::Blockstorage block(blocks_->Root() / name_of_shard);
    return block.Has(cid);
  }

  logger_->error("Repo is closed");
  return false;
}

template <typename StoreValue>
bool Repo<StoreValue>::Exists() noexcept {
  return std::filesystem::exists(root_->Root()) &&
         std::filesystem::exists(root_->Root() / "blocks");
}

template <typename StoreValue>
Status Repo<StoreValue>::deleteUnmarkedBlock() noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  Pins pins_set = pinner_->PinSet();
  std::filesystem::path to_delete;
  for (auto const& element :
       std::filesystem::recursive_directory_iterator(blocks_->Root())) {
    std::filesystem::path path = element.path();
    if (element.is_directory()) {
      if (is_empty(path)) {
        to_delete = path;
      }
    } else {
      common::Cid cid(path.filename().string());
      if (!pinner_->IsPinned(cid)) {
        Status is_deleted = deleteByKey(cid);
        if (!is_deleted.ok()) {
          logger_->error(is_deleted.error_message());
        }
        logger_->info("GC DELETED BLOCK {}", cid.ToString());
        break;
      }
    }
  }
  std::filesystem::remove(to_delete);
}

template <typename StoreValue>
size_t Repo<StoreValue>::getDirSize(
    const std::filesystem::path& dir_path) const noexcept {
  size_t size = 0;
  for (auto& dir_entry :
       std::filesystem::recursive_directory_iterator(dir_path)) {
    if (!dir_entry.is_directory()) {
      size += dir_entry.file_size();
    }
  }
  return size;
}

template <typename StoreValue>
void Repo<StoreValue>::startGarbageCollector() noexcept {
  std::unique_lock<std::mutex> lock_guard(mutex_);
  if (!pinner_->Exists()) {
    return;
  }

  while (is_running_) {
    cv_.wait_until(lock_guard,
                   std::chrono::steady_clock::now() +
                       common::utils::ToTime(config_.Get("gc_time")).first);

    while (common::utils::ToBytes(config_.Get("gc_size")).first <
           getDirSize(std::filesystem::path(blocks_->Root()))) {
      Status status = deleteUnmarkedBlock();
      if (!status.ok()) {
        break;
      }
    }
  }
}

}  // namespace repo
}  // namespace cognitio
