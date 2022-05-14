// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <stack>

#include "linked_data/merkle_dag.hpp"

namespace cognitio {
namespace linked_data {

DagNode::DagNode(std::vector<uint8_t> &&bytes,
                 const std::vector<DagNode> &children_data) {
  data_.SetData(std::move(bytes));
  for (DagNode node : children_data) {
    children_.insert(std::pair<common::Cid, DagNode>(GetCid(), node));
  }
}

Status MerkleDag::AddNode(const DagNode &node) {
  return block_service_->Put(Block(node.GetCid(), node));
}

std::pair<Status, common::Cid> MerkleDag::Add(
    const std::vector<std::vector<uint8_t>> &chunks) {
  if (!merkle_root_) {
    merkle_root_ = buildGraph(chunks);
  }
  return std::pair<Status, common::Cid>(
      block_service_->Put(Block(merkle_root_->GetCid(), *merkle_root_)),
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

// !!!TODO: implement is_recursive deleting nodes
Status MerkleDag::RemoveNode(const cognitio::common::Cid &cid,
                             bool is_recursive = true) {
  if (!is_recursive) {
    return block_service_->Delete(cid);
  }
  DagNode to_delete_node = GetNode(cid).second;
  std::vector<DagNode> to_delete_vec = DirectedTrasersal(to_delete_node);
  
  for (DagNode node : to_delete_vec) {
    block_service_->Delete(node.GetCid());
  }
  block_service_->Delete(to_delete_node.GetCid());
}

std::vector<DagNode> MerkleDag::DirectedTrasersal(DagNode root_node)
    const {
  std::vector<DagNode> result_vec;
  std::stack<DagNode> node_st;
  DagNode current_node = root_node;

  do {
    for (auto node : current_node.GetChildren()) {
      result_vec.push_back(node.second);
      node_st.push(node.second);
    }
    current_node = node_st.top();
    node_st.pop();
  } while (!node_st.empty());

  return result_vec;
}

std::unique_ptr<DagNode> MerkleDag::buildGraph(const std::vector<std::vector<uint8_t>> &chunks) {
  /* initalizing the bottom lay of chunks */
  int vec_size;
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
  std::vector<std::vector<DagNode>> layer_vec(bottom_lay_vec.size());
  std::vector<std::vector<DagNode>> previous_layer_vec =
      std::move(bottom_lay_vec);
  size_t blck_cnt = 0;
  while (previous_layer_vec.size() != 1) {
    for (auto chunk_block : previous_layer_vec) {
      std::vector<uint8_t> concatenated_data;
      for (auto chunk : chunk_block) {
        concatenated_data.insert(concatenated_data.end(),
                                 chunk.GetCid().GetBytes().begin(),
                                 chunk.GetCid().GetBytes().end());
      }

      DagNode parent_node(std::move(concatenated_data), chunk_block);
      layer_vec[blck_cnt].push_back(parent_node);
      if (blck_cnt == CHUNK_SIZE) {
        ++blck_cnt;
      }
    }
    previous_layer_vec = std::move(layer_vec);
    if (previous_layer_vec.size() < CHUNK_SIZE) {
      layer_vec.resize(previous_layer_vec.size());
    } else {
      layer_vec.resize((previous_layer_vec.size() % CHUNK_SIZE == 0)
                           ? previous_layer_vec.size()
                           : previous_layer_vec.size() + 1);
    }
    blck_cnt = 0;
  }
  return std::make_unique<DagNode>(previous_layer_vec[0][0]);
}

}  // namespace linked_data
}  // namespace cognitio
