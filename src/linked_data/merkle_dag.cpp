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

Status MerkleDag::AddNode(const cognitio::linked_data::Node &node) {
  std::vector<uint8_t> bytes = node.GetContent();
  return block_service_->Put(node.GetCid(), bytes);
}

std::pair<Status, cognitio::linked_data::Node> MerkleDag::GetNode(
    const cognitio::common::Cid &cid) const {
  std::pair<Status, std::vector<uint8_t>> node = block_service_->Get(cid);
  if (node.first.ok()) {
    return std::pair<Status, linked_data::Node>(Status(),
                                                std::move(node.second));
  }
  return std::pair<Status, linked_data::Node>(
      Status(StatusCode::CANCELLED, "This node doesnt exist"), Node());
}

Status MerkleDag::RemoveNode(const cognitio::common::Cid &cid) {
  return block_service_->Delete(cid);
}



}  // namespace linked_data
}  // namespace cognitio