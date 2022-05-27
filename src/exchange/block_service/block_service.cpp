#include "exchange/block_service/block_service.hpp"

namespace cognitio {
namespace exchange {

using namespace datastore;

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
      std::make_shared<repo::Repo<std::string>>(repo::Repo<std::string>(path));
  status_open = repo_->Init();
  if (status_open.ok()) {
    closed_ = false;
  }
  return status_open;
}

// Status BlockService::Close() noexcept { return repo_->Close(); }

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

std::string BlockService::createMeta(
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

Status BlockService::Put(const ProtoBlock& block) noexcept {
  // is_daemon_opened_
  //             ? block_swap_->Put(block.GetCid(),
  //             block.GetNode().GetContent()) : repo_->Add(block.GetCid(),
  //             block.GetNode().GetContent())
  std::string meta_data = createMeta(block.GetNode());
  std::vector<uint8_t> meta(meta_data.cbegin(), meta_data.cend());

  std::vector<uint8_t> content = block.GetNode().GetContent();

  meta.insert(meta.end(), content.cbegin(), content.cend());

  return repo_->Add(block.GetCid(), meta);
}

std::vector<linked_data::DagNode> BlockService::getMeta(
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

linked_data::ProtoBlock BlockService::Get(
    const common::Cid& key) const noexcept {
  std::vector<uint8_t> content = repo_->Get(key);

  std::vector<linked_data::DagNode> children =
      getMeta({content.cbegin(), content.cend()});

  ProtoBlock block;
  block.SetNode(linked_data::DagNode(children));
  block.SetCid(key);

  return block;
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
