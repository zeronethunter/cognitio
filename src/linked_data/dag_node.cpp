// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/dag_node.hpp"

#include <span>

namespace cognitio {
namespace linked_data {

DagNode::DagNode(std::vector<uint8_t> &&bytes,
                 const std::vector<DagNode> &children_data) {
  data_.SetData(bytes);
  for (const auto &node : children_data) {
    children_.push_back(std::make_pair<common::Cid, std::shared_ptr<DagNode>>(
        GetCid(), std::make_shared<DagNode>(node)));
  }
}

DagNode::DagNode(const std::vector<DagNode> &children) {
  for (const auto &node : children) {
    children_.push_back(std::make_pair<common::Cid, std::shared_ptr<DagNode>>(
        GetCid(), std::make_shared<DagNode>(node)));
  }
}

std::vector<uint8_t> DagNode::GetContent() const { return data_.GetData(); }

size_t DagNode::Count() const { return children_.size(); }

common::Cid DagNode::GetCid() const {
  auto content = GetContent();
  return common::Cid(content);
}
Status DagNode::DecodeProtoNode(const Node &node) {
  data_ = node.data();
  children_.resize(0);

  for (const auto &child : node.cid()) {
    children_.emplace_back(common::Cid(child),
                           std::make_shared<DagNode>(DagNode()));
  }

  return Status::OK;
}

}  // namespace linked_data
}  // namespace cognitio
