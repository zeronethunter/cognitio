#include "exchange/block_service/block_service.hpp"

#include "common/status.hpp"
#include "common/status_code.hpp"
#include "linked_data/proto_block.hpp"

namespace cognitio {
namespace exchange {

using namespace datastore;

Status BlockService::Open(const std::filesystem::path& path) noexcept {
  repo_ =
      std::make_shared<repo::Repo<std::string>>(repo::Repo<std::string>(path));

  auto status_open = repo_->Init();
  if (status_open.ok()) {
    closed_ = false;
  }
  return status_open;
}

std::filesystem::path BlockService::Root() const noexcept {
  if (repo_->Exists()) {
    return repo_->Root();
  }

  logger_->error("Can't get root. Open repo first.");
  return {};
}

Status BlockService::Put(const ProtoBlock& block) noexcept {
  if (is_daemon_opened_) {
    logger_->debug("Adding block to dht");
    if (!block_swap_->Add(block.GetCid()).ok()) {
      logger_->warn("Failed to put block to dht");
    }
  }

  return repo_->Add(block);
}

linked_data::ProtoBlock BlockService::Get(
    const common::Cid& key) const noexcept {
  linked_data::ProtoBlock bl;
  if (!closed_) {
    bl = repo_->Get(key);
    if (!bl.IsInitialized()) {
      bl = block_swap_->Get(key);
    }
  }

  return bl;
}

Status BlockService::Delete(const common::Cid& key) noexcept {
  if (!closed_) {
    return repo_->Delete(key);
  }
  return {StatusCode::FAILED, "Failed to store block."};
}

bool BlockService::Has(const common::Cid& key) const noexcept {
  return repo_->Has(key);
}

Status BlockService::PutMany(const std::vector<ProtoBlock>& source) noexcept {
  for (auto const& block : source) {
    Status status_put = Put(block);
    if (!status_put.ok()) {
      return status_put;
    }
  }

  return Status::OK;
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

  return Status::OK;
}

}  // namespace exchange
}  // namespace cognitio
