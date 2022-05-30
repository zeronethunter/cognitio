// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/merkle_dag.hpp"

#include <queue>
#include <stack>
#include <vector>

#include "linked_data/dag_node.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace linked_data {

std::pair<Status, common::Cid> MerkleDag::Add(
    const std::vector<std::vector<uint8_t>> &chunks) {
  logger_->debug("Adding file is starting...");
  if (!merkle_root_) {
    merkle_root_ = buildGraph(chunks);
  }

  std::vector<ProtoBlock> blocks_vec = collectBlocks(*merkle_root_);
  for (const ProtoBlock &block : blocks_vec) {
    Status status_code = block_service_->Put(block);
    if (!status_code.ok()) {
      if (status_code.error_code() == StatusCode::ALREADY_EXISTS) {
        logger_->warn("This block is already exists.",
                      status_code.error_message());
        continue;
      }

      return std::pair<Status, common::Cid>(status_code, common::Cid());
    }
  }

  return std::pair<Status, common::Cid>(Status::OK, merkle_root_->GetCid());
}

std::pair<Status, std::vector<uint8_t>> MerkleDag::Get(
    const common::Cid &cid) const {
  logger_->debug("Starting getting node.");

  /* getting root node from unix fs */
  std::pair<Status, DagNode> root_getter = getNode(cid);

  if (!root_getter.first.ok()) {
    logger_->error("Cannot find Node with cid: {}", cid.ToString());
    return std::pair<Status, std::vector<uint8_t>>(StatusCode::FAILED,
                                                   std::vector<uint8_t>());
  }

  logger_->debug("Found root node successfully.");

  std::vector<DagNode> collected_nodes;
  std::string concatenated_bytes = "";
  std::queue<DagNode> node_queue;

  for (const auto &element : root_getter.second.GetChildren()) {
    node_queue.push(getNode(element.first).second);
  }

  while (!node_queue.empty()) {
    DagNode current_node = node_queue.front();
    node_queue.pop();

    if (!current_node.GetChildren().empty()) {
      for (const auto &child : current_node.GetChildren()) {
        node_queue.push(getNode(child.first).second);
      }
    }

    if (!current_node.GetContent().empty()) {
      for (const auto x : current_node.GetContent()) {
        concatenated_bytes += static_cast<char>(x);
      }
    }
  }

  return std::pair<Status, std::vector<uint8_t>>(
      Status::OK, std::vector<uint8_t>(concatenated_bytes.begin(),
                                       concatenated_bytes.end()));
}

std::pair<Status, DagNode> MerkleDag::getNode(
    const cognitio::common::Cid &cid) const {
  DagNode node = block_service_->Get(cid).GetNode();
  if (!node.GetCid().ToString().empty()) {
    return std::pair<Status, DagNode>(Status(), std::move(node));
  }
  return std::pair<Status, linked_data::DagNode>(
      Status(StatusCode::CANCELLED, "This node doesnt exist"), DagNode());
}

Status MerkleDag::Remove(const common::Cid &cid, bool is_recursive) {
  if (!is_recursive) {
    return block_service_->Delete(cid);
  }

  DagNode root_node = getNode(cid).second;
  std::queue<DagNode> node_queue;
  std::vector<ProtoBlock> to_delete_vec = collectBlocks(root_node);

  for (const auto &element : root_node.GetChildren()) {
    node_queue.push(getNode(element.first).second);
  }

  if (!block_service_->Delete(to_delete_vec.begin()->GetCid()).ok()) {
    return Status(StatusCode::FAILED, "Can't find node with this CID");
  }
  to_delete_vec.erase(to_delete_vec.begin());

  while (!node_queue.empty()) {
    DagNode current_node = node_queue.front();
    node_queue.pop();

    if (!current_node.GetChildren().empty()) {
      for (const auto &child : current_node.GetChildren()) {
        node_queue.push(getNode(child.first).second);
      }
    }

    if (!block_service_->Delete(current_node.GetCid()).ok()) {
      return Status(StatusCode::FAILED, "Can't find node with this CID");
    }
  }

  return Status::OK;
}

std::vector<ProtoBlock> MerkleDag::collectBlocks(
    const DagNode &root_node) const {
  std::vector<ProtoBlock> result_vec;
  std::queue<std::pair<common::Cid, DagNode>> node_queue;
  //  result_vec.emplace_back(root_node.GetCid(), root_node);

  if (root_node.GetChildren().empty()) {
    return result_vec;
  }

  /* inserting root block to result vec */
  result_vec.insert(result_vec.begin(),
                    ProtoBlock(root_node.GetCid(), root_node));

  /* initializing first layer after root */
  std::vector<std::pair<common::Cid, std::shared_ptr<DagNode>>>
      first_layer_vec = root_node.GetChildren();

  /* pushing all nodes from first layer in stack */
  std::for_each(first_layer_vec.begin(), first_layer_vec.end(),
                [&](const auto &node) {
                  node_queue.push(std::make_pair(node.first, *node.second));
                });

  do {
    /* taking top of queue */
    std::pair<common::Cid, DagNode> current_node = node_queue.front();

    /* pushing this node in proto block */
    result_vec.emplace_back(current_node.first, current_node.second);

    /* watching for children of current node */
    for (const auto &node : current_node.second.GetChildren()) {
      node_queue.push(std::make_pair(node.first, *node.second));
    }
    /* popping current node from queue */
    node_queue.pop();
  } while (!node_queue.empty());

  return result_vec;
}

std::unique_ptr<DagNode> MerkleDag::buildGraph(
    const std::vector<std::vector<uint8_t>> &chunks) {
  /* initalizing the bottom lay of chunks */
  size_t vec_size;
  if (chunks.size() < CHUNK_SIZE) {
    vec_size = 1;
  } else {
    vec_size = (chunks.size() % CHUNK_SIZE == 0)
                   ? chunks.size() / CHUNK_SIZE
                   : chunks.size() / CHUNK_SIZE + 1;
  }

  std::vector<std::vector<DagNode>> bottom_lay_vec(vec_size);
  int offset_cnt = 0;
  int vec_indx = 0;
  for (auto chunk : chunks) {
    bottom_lay_vec[static_cast<unsigned long>(vec_indx)].push_back(
        DagNode(std::move(chunk)));
    if (++offset_cnt == CHUNK_SIZE) {
      offset_cnt = 0;
      ++vec_indx;
    }
  }

  /* build nodes above bottom lay */
  std::vector<std::vector<DagNode>> layer_vec(
      (bottom_lay_vec.size() % CHUNK_SIZE == 0)
          ? bottom_lay_vec.size() / CHUNK_SIZE
          : bottom_lay_vec.size() / CHUNK_SIZE + 1);
  std::vector<std::vector<DagNode>> previous_layer_vec =
      std::move(bottom_lay_vec);
  size_t blck_cnt = 0;

  while (layer_vec.size() != 1) {
    for (const auto &chunk_block : previous_layer_vec) {
      DagNode parent_node(chunk_block);

      layer_vec[blck_cnt].push_back(parent_node);
      if (layer_vec[blck_cnt].size() == CHUNK_SIZE) {
        ++blck_cnt;
      }
    }
    previous_layer_vec = std::move(layer_vec);

    if (previous_layer_vec.size() < CHUNK_SIZE) {
      layer_vec.resize(1);
    } else {
      layer_vec.resize((previous_layer_vec.size() % CHUNK_SIZE == 0)
                           ? previous_layer_vec.size()
                           : previous_layer_vec.size() + 1);
    }
    blck_cnt = 0;
  }

  std::for_each(
      previous_layer_vec.begin(), previous_layer_vec.end(),
      [&](const auto &block) { layer_vec[0].push_back(DagNode(block)); });

  return std::make_unique<DagNode>(layer_vec[0]);
}

}  // namespace linked_data
}  // namespace cognitio
