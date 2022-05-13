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

Status MerkleDag::buildGraph(const std::vector<std::vector<uint8_t>> &chunks) {
  /* initalizing the bottom lay of chunks */
  int vec_size = (chunks.size() % CHUNK_SIZE == 0)
                     ? chunks.size() / CHUNK_SIZE
                     : chunks.size() / CHUNK_SIZE + 1;
  std::vector<DagNode> bottom_lay_vec(vec_size);

  int offset_cnt = 0;
  int vec_indx = 0;
  for (auto chunk : chunks) {
    bottom_lay_vec[vec_indx] = std::move(chunk);
    if (++offset_cnt == CHUNK_SIZE) {
      offset_cnt = 0;
      ++vec_indx;
    }
  }
}

}  // namespace linked_data
}  // namespace cognitio