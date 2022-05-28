//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/repo.hpp"

#include <cstdint>
#include <filesystem>
#include <ios>
#include <sstream>
#include <type_traits>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "config/config.hpp"
#include "repo/gc/gc.hpp"

namespace cognitio {
namespace repo {

template <typename StoreValue>
void Repo<StoreValue>::initRepoStorage(
    const std::filesystem::path& path) noexcept {
  datastore::Filesystem<StoreValue> root(path);
  root_ = std::make_unique<datastore::Filesystem<StoreValue>>(root);

  blockstorage::Blockstorage blocks(path / "blocks");
  blocks_ = std::make_unique<blockstorage::Blockstorage>(std::move(blocks));

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

  Status status_init = config_.Dump();

  return status_init;
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::filesystem::path& path) noexcept
    : config_(config::Config::GetInstance()) {
  initRepoStorage(path);
  logger_ = common::logger::createLogger("repo");
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::string& name) noexcept
    : config_(config::Config::GetInstance()) {
  initRepoStorage(std::filesystem::path(name));
  logger_ = common::logger::createLogger("repo");
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
std::string Repo<StoreValue>::shard(const cognitio::common::Cid& cid,
                                    size_t name_length) const noexcept {
  std::string str_cid = cid.ToString();
  size_t offset = str_cid.length() - name_length;
  return {str_cid.cbegin() + static_cast<long>(offset), str_cid.cend()};
}

template <typename StoreValue>
Status Repo<StoreValue>::Add(const ProtoBlock& block) noexcept {
  logger_->debug("Adding block to repo.");
  std::unique_ptr<Block> proto_block = block.ToProtoMessage();

  std::stringstream dump(std::ios_base::out | std::ios_base::trunc |
                         std::ios_base::binary);

  proto_block->SerializeToOstream(&dump);
  std::string dump_str = dump.str();
  if (dump_str.empty()) {
    logger_->warn("Block is empty.");
  }

  logger_->debug("Successfully added block.");
  return addByKey(block.GetCid(), {dump_str.begin(), dump_str.end()});
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

    proto_block->ParseFromIstream(&data);

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
Status Repo<StoreValue>::addByKey(const common::Cid& cid,
                                  const std::vector<uint8_t>& data) noexcept {
  std::string name_of_shard = shard(cid);

  blockstorage::Blockstorage block(blocks_->Root() / name_of_shard /
                                   cid.ToString());
  Status is_opened = block.Open();

  if (!is_opened.ok()) {
    return is_opened;
  }

  Status is_added = block.Put(cid, data);

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

    Status is_deleted = block.Delete(cid);

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
    std::pair<Status, std::vector<uint8_t>> result = block.Get(cid);

    if (!result.first.ok()) {
      logger_->error(result.first.error_message());
      return {};
    }

    return block.Get(cid).second;
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

}  // namespace repo
}  // namespace cognitio
