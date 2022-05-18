#include "exchange/block_service/block_service.hpp"
#include "datastore/ds_fs.hpp"

namespace cognitio {
namespace exchange {
Status BlockService::Open(const std::filesystem::path& path,
                          bool is_daemon_opened) noexcept {
  Status status_open;
  is_daemon_opened_ = is_daemon_opened;
  //  if (is_daemon_opened_) {
  //    status_open = block_swap_->Open();
  //    if (status_open.ok()) {
  //      closed_ = false;
  //    }
  //    return status_open;
  //  }
  repo_ =
      std::make_unique<repo::Repo<std::string>>(repo::Repo<std::string>(path));
  status_open = repo_->Init();
  if (status_open.ok()) {
    closed_ = false;
  }
  return status_open;
}

Status BlockService::Close() noexcept {}

std::filesystem::path BlockService::Root() const noexcept {
  if (!is_daemon_opened_) {
    if (repo_->Exists()) {
      return repo_->Root();
    }
    logger_->error("Can't get root. Open repo first.");
    return {};
  }
  logger_->info("Daemon is opened. Can't get root.");
  return {};
}

Status BlockService::Put(const ProtoBlock& block) noexcept {
  // is_daemon_opened_
  //             ? block_swap_->Put(block.GetCid(),
  //             block.GetNode().GetContent()) : repo_->Add(block.GetCid(),
  //             block.GetNode().GetContent())
  return repo_->Add(block.GetCid(), block.GetNode().GetContent());
}

linked_data::ProtoBlock BlockService::Get(
    const common::Cid& key) const noexcept {
  //  std::vector<uint8_t> bytes =
  //      is_daemon_opened_ ? block_swap_->Get(key) : repo_->Get(key);
  std::vector<uint8_t> bytes = repo_->Get(key);
  return ProtoBlock(key, linked_data::DagNode(std::move(bytes)));
}

Status BlockService::Delete(const common::Cid& key) noexcept {
  //    return is_daemon_opened_ ? block_swap_->Delete(key) :
  //    repo_->Delete(key);
  return repo_->Delete(key);
}

bool BlockService::Has(const common::Cid& key) const noexcept {
  //    return is_daemon_opened_ ? block_swap_->Has(key) : repo_->Has(key);
  return repo_->Has(key);
}

Status BlockService::PutMany(const std::vector<ProtoBlock>& source) noexcept {
  for (auto const& block : source) {
    Status status_put = Put(block);
    if (!status_put.ok()) {
      return status_put;
    }
  }
}

std::vector<linked_data::ProtoBlock> BlockService::GetMany(
    const std::vector<common::Cid>& source) const noexcept {
  std::vector<ProtoBlock> result;
  for (auto const& key : source) {
    ProtoBlock value = Get(key);
    result.push_back(value);
  }
  return result;
}

Status BlockService::DeleteMany(
    const std::vector<common::Cid>& source) noexcept {
  for (auto const& key : source) {
    Status status_put = Delete(key);
    if (!status_put.ok()) {
      return status_put;
    }
  }
}
}  // namespace exchange
}  // namespace cognitio
