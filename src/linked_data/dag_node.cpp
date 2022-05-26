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

std::unique_ptr<Node> DagNode::EncodeProtoNode() const {
  Node package_node;
  package_node.set_data(data_.EncodeMessage());

  for (size_t i = 0; i < children_.size(); ++i) {
    package_node.set_cid(i, children_[i].first.ToString());
  }
  return std::make_unique<Node>(package_node);
}

}  // namespace linked_data
}  // namespace cognitio
