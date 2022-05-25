// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/merkle_dag.hpp"

#include <stack>

namespace cognitio {
namespace linked_data {

Status MerkleDag::AddNode(const DagNode &node) {
  return block_service_->Put(ProtoBlock(node.GetCid(), node));
}

std::pair<Status, common::Cid> MerkleDag::Add(
    const std::vector<std::vector<uint8_t>> &chunks) {
  if (!merkle_root_) {
    merkle_root_ = buildGraph(chunks);
  }
  return std::pair<Status, common::Cid>(
      block_service_->Put(ProtoBlock(merkle_root_->GetCid(), *merkle_root_)),
      merkle_root_->GetCid());
}

std::pair<Status, DagNode> MerkleDag::GetNode(
    const cognitio::common::Cid &cid) const {
  std::vector<uint8_t> bytes = block_service_->Get(cid).GetNode().GetContent();
  DagNode node(std::move(bytes));
  if (!node.GetCid().ToString().empty()) {
    return std::pair<Status, DagNode>(Status(), std::move(node));
  }
  return std::pair<Status, linked_data::DagNode>(
      Status(StatusCode::CANCELLED, "This node doesnt exist"), DagNode());
}

Status MerkleDag::RemoveNode(const common::Cid &cid, bool is_recursive) {
  if (!is_recursive) {
    return block_service_->Delete(cid);
  }
  DagNode to_delete_node = GetNode(cid).second;
  std::vector<DagNode> to_delete_vec = DirectedTrasersal(to_delete_node);

  for (DagNode node : to_delete_vec) {
    if (!block_service_->Delete(node.GetCid()).ok()) {
      return Status(StatusCode::FAILED, "Can't find node with this CID");
    };
  }
  if (!block_service_->Delete(to_delete_node.GetCid()).ok()) {
    return Status(StatusCode::FAILED, "Can't find node with this CID");
  }
  return Status();
}

std::vector<DagNode> MerkleDag::DirectedTrasersal(
    const DagNode &root_node) const {
  std::vector<DagNode> result_vec;
  std::stack<DagNode> node_st;
  DagNode current_node = root_node;

  do {
    for (const auto &node : current_node.GetChildren()) {
      result_vec.push_back(*node.second);
      node_st.push(*node.second);
    }
    current_node = node_st.top();
    node_st.pop();
  } while (!node_st.empty());

  return result_vec;
}

std::unique_ptr<DagNode> MerkleDag::buildGraph(
    const std::vector<std::vector<uint8_t>> &chunks) {
  /* initalizing the bottom lay of chunks */
  size_t vec_size;
  if (chunks.size() < CHUNK_SIZE) {
    vec_size = chunks.size();
  } else {
    vec_size = (chunks.size() % CHUNK_SIZE == 0)
                   ? chunks.size() / CHUNK_SIZE
                   : chunks.size() / CHUNK_SIZE + 1;
  }

  std::vector<std::vector<DagNode>> bottom_lay_vec(vec_size);
  int offset_cnt = 0;
  int vec_indx = 0;
  for (auto chunk : chunks) {
    bottom_lay_vec[vec_indx].push_back(DagNode(std::move(chunk)));
    if (++offset_cnt == CHUNK_SIZE) {
      offset_cnt = 0;
      ++vec_indx;
    }
  }

  /* build nodes above bottom lay */
  std::vector<std::vector<DagNode>> layer_vec(1);
  std::vector<std::vector<DagNode>> previous_layer_vec =
      std::move(bottom_lay_vec);
  std::vector<DagNode> last_layer;
  size_t blck_cnt = 0;
  while (last_layer.size() != 1) {
    for (const auto &chunk_block : previous_layer_vec) {
      DagNode parent_node(chunk_block);
      std::vector<uint8_t> concatenated_data;
      auto children = parent_node.GetChildren();

      for (auto &child : children) {
        std::vector<uint8_t> content = child.second->GetContent();
        concatenated_data.insert(concatenated_data.end(), content.begin(),
                                 content.end());
      }

      parent_node.SetData(concatenated_data);

      layer_vec[blck_cnt].push_back(parent_node);
      if (parent_node.GetChildren().size() != CHUNK_SIZE) {
        ++blck_cnt;
      }
    }
    previous_layer_vec = std::move(layer_vec);

    last_layer = previous_layer_vec[previous_layer_vec.size() - 1];

    if (last_layer.size() < CHUNK_SIZE) {
      layer_vec.resize(1);
    } else {
      layer_vec.resize((last_layer.size() % CHUNK_SIZE == 0)
                           ? last_layer.size()
                           : last_layer.size() + 1);
    }
    blck_cnt = 0;
  }
  return std::make_unique<DagNode>(previous_layer_vec[0][0]);
}

}  // namespace linked_data
}  // namespace cognitio
