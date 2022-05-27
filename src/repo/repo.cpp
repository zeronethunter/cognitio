//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/repo.hpp"

#include <cstdint>
#include <filesystem>

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
  logger_ = common::logger::createLogger("Repo logger");
}

template <typename StoreValue>
Repo<StoreValue>::Repo(const std::string& name) noexcept
    : config_(config::Config::GetInstance()) {
  initRepoStorage(std::filesystem::path(name));
  logger_ = common::logger::createLogger("Repo logger");
}

template <typename StoreValue>
Status Repo<StoreValue>::Init() noexcept {
  Status err = Status::OK;
  if (!root_) {
    logger_->error("I don't know where to init repository.");
    return Status::FAILED;
  }

  if (!Exists()) {
    initRepoStorage(root_->Root());
    err = openRepo();
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
std::string Repo<StoreValue>::createMeta(
    const linked_data::DagNode& node) const noexcept {
  std::string cid = node.GetCid().ToString();

  auto children_nodes = node.GetChildren();

  std::string meta;

  std::for_each(
      children_nodes.cbegin(), children_nodes.cend(),
      [&meta](auto const& pair) { meta += pair.first.ToString() + ' '; });

  meta[meta.length() - 1] = '\n';

  return meta;
}

template <typename StoreValue>
Status Repo<StoreValue>::Add(const ProtoBlock& block) noexcept {
  // is_daemon_opened_
  //             ? block_swap_->Put(block.GetCid(),
  //             block.GetNode().GetContent()) : repo_->Add(block.GetCid(),
  //             block.GetNode().GetContent())
  std::string meta_data = createMeta(block.GetNode());
  std::vector<uint8_t> meta(meta_data.cbegin(), meta_data.cend());

  std::vector<uint8_t> content = block.GetNode().GetContent();

  meta.insert(meta.end(), content.cbegin(), content.cend());

  return addByKey(block.GetCid(), meta);
}

template <typename StoreValue>
std::vector<linked_data::DagNode> Repo<StoreValue>::getMeta(
    const std::string& content) const noexcept {
  std::string meta = content.substr(0, content.find('\n') - 1);
  meta += ' ';

  std::vector<linked_data::DagNode> result;

  size_t pos;
  while ((pos = meta.find(' ')) != std::string::npos) {
    common::Cid cid(meta.substr(0, pos - 1));
    meta.erase(pos);
    result.emplace_back(cid.GetBytes());
  }

  return result;
}

template <typename StoreValue>
linked_data::ProtoBlock Repo<StoreValue>::Get(
    const common::Cid& key) const noexcept {
  std::vector<uint8_t> content = getByKey(key);

  std::vector<linked_data::DagNode> children =
      getMeta({content.cbegin(), content.cend()});

  ProtoBlock block;
  block.SetNode(linked_data::DagNode(children));
  block.SetCid(key);

  return block;
}

template <typename StoreValue>
Status Repo<StoreValue>::addByKey(const common::Cid& cid,
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
  return {StatusCode::CANCELLED, "Open repo before adding."};
}

template <typename StoreValue>
Status Repo<StoreValue>::deleteByKey(const common::Cid& cid) noexcept {
  if (!closed_) {
    std::string name_of_shard = shard(cid);

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard)) {
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

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard)) {
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

    if (!std::filesystem::exists(blocks_->Root() / name_of_shard)) {
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
