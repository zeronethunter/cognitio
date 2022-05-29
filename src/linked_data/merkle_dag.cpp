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
      // Cannot add bytes to string not like that. It's falling
      // with the exception std::length_error in case of using
      // insert, append or constructor with iterators
      for (const auto x : current_node.GetContent()) {
        concatenated_bytes += static_cast<char>(x);
      }

      // concatenated_bytes.append(current_node.GetContent().begin(),
      //                        current_node.GetContent().end());
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
  std::vector<ProtoBlock> to_delete_vec = collectBlocks(root_node);

  for (ProtoBlock node : to_delete_vec) {
    if (!block_service_->Delete(node.GetCid()).ok()) {
      return Status(StatusCode::FAILED, "Can't find node with this CID");
    }
  }

  return Status::OK;
}

std::vector<DagNode> MerkleDag::getSubNodes(const DagNode &root) const {
  std::vector<DagNode> collected_nodes({root});
  std::stack<DagNode> dag_st({root});

  do {
    DagNode current_node = dag_st.top();
    dag_st.pop();

    // auto got_node = GetNode(current_node.GetCid());
    std::vector<DagNode> children_vec = CollectNodes(current_node);

    /* push children in stack */
    std::for_each(children_vec.rbegin(), children_vec.rend(),
                  [&](const DagNode &node) { dag_st.push(node); });

    /* push nodes in result vector */
    std::for_each(
        children_vec.begin(), children_vec.end(),
        [&](const DagNode &node) { collected_nodes.push_back(node); });
  } while (!dag_st.empty());

  return collected_nodes;
}

std::vector<DagNode> MerkleDag::CollectNodes(const DagNode &root_node) const {
  std::vector<DagNode> result_vec;

  std::queue<DagNode> node_queue;

  if (root_node.GetChildren().empty()) {
    return result_vec;
  }

  /* pushing all nodes from first layer in stack */
  std::for_each(root_node.GetChildren().rbegin(),
                root_node.GetChildren().rend(),
                [&](const auto &node) { node_queue.push(*node.second); });

  do {
    /* taking top of queue */
    DagNode current_node = node_queue.front();

    /* pushing this node in proto block */
    result_vec.push_back(current_node);

    /* watching for children of current node */
    for (const auto &node : current_node.GetChildren()) {
      node_queue.push(*node.second);
    }
    /* popping current node from queue */
    node_queue.pop();
  } while (!node_queue.empty());

  return result_vec;
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
  std::vector<std::vector<DagNode>> layer_vec(1);
  std::vector<std::vector<DagNode>> previous_layer_vec =
      std::move(bottom_lay_vec);
  size_t blck_cnt = 0;

  while (previous_layer_vec.size() != 1) {
    for (const auto &chunk_block : previous_layer_vec) {
      DagNode parent_node(chunk_block);
      //      std::vector<uint8_t> concatenated_data;
      auto children = parent_node.GetChildren();

      //      for (auto &child : children) {
      //        std::vector<uint8_t> content = child.second->GetContent();
      //        concatenated_data.insert(concatenated_data.end(),
      //        content.begin(),
      //                                 content.end());
      //      }
      //
      //      parent_node.SetData(concatenated_data);

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

  DagNode parent_root(previous_layer_vec[0]);

  //  std::vector<uint8_t> concatenated_bytes;
  //  for (auto &child : previous_layer_vec[0]) {
  //    std::vector<uint8_t> content = child.GetContent();
  //    concatenated_bytes.insert(concatenated_bytes.end(), content.begin(),
  //                              content.end());
  //  }

  //  merkle_root_ = std::make_unique<DagNode>(exchange::BlockService(),
  //  previous_layer_vec[0]);

  return std::make_unique<DagNode>(parent_root);
}

}  // namespace linked_data
}  // namespace cognitio
