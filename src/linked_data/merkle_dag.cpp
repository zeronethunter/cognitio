// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/merkle_dag.hpp"

namespace cognitio {
namespace linked_data {

MerkleDag::MerkleDag(const repo::blockstorage::Blockstorage &blocks) {
  block_service_ =
      std::move(std::make_shared<repo::blockstorage::Blockstorage>(blocks));
}

Status MerkleDag::AddNode(const cognitio::linked_data::DagNode &node) {
  std::vector<uint8_t> bytes = node.GetContent();
  return block_service_->Put(node.GetCid(), bytes);
}

std::pair<Status, cognitio::linked_data::DagNode> MerkleDag::GetNode(
    const cognitio::common::Cid &cid) const {
  std::pair<Status, std::vector<uint8_t>> node = block_service_->Get(cid);
  if (node.first.ok()) {
    return std::pair<Status, linked_data::DagNode>(Status(),
                                                std::move(node.second));
  }
  return std::pair<Status, linked_data::DagNode>(
      Status(StatusCode::CANCELLED, "This node doesnt exist"), DagNode());
}

Status MerkleDag::RemoveNode(const cognitio::common::Cid &cid) {
  return block_service_->Delete(cid);
}

std::shared_ptr<linked_data::DagNode> MerkleDag::FetchGraph(
    const common::Cid &cid) const {
  linked_data::DagNode node(block_service_->Get(cid).second);
}

Status MerkleDag::build_graph(
    std::function<std::shared_ptr<Node>(const common::Cid &cid)>
        node_getter,
    const std::shared_ptr<DagNode> &root,
    const std::vector<std::unique_ptr<Link>> &links,
    std::optional<size_t> max_depth, 
    size_t current_depth) {
  if (max_depth && current_depth == *max_depth) {
    return Status();
  }

  for (const auto &link : links) {
    auto request = node_getter(link.get()->GetCid());

    std::shared_ptr<Node> node = request;
    auto child_node = std::make_shared<linked_data::Node>(node->GetContent());
  }
}

}  // namespace linked_data
}  // namespace cognitio