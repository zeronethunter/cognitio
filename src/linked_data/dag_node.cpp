// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/dag_node.hpp"

#include <span>

namespace cognitio {
namespace linked_data {

DagNode::DagNode(std::vector<uint8_t> &bytes,
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
  files::unixfs::UnixFS unixfs_data;
  unixfs_data.DecodeMessage(node.data());
  data_ = unixfs_data;
  children_.resize(0);

  for (const auto &child : node.cid()) {
    children_.emplace_back(common::Cid(child),
                           std::make_shared<DagNode>(DagNode()));
  }

  return Status::OK;
}

std::unique_ptr<Node> DagNode::EncodeProtoNode() const {
  Node package_node;
  package_node.set_allocated_data(data_.EncodeMessage().get());
  std::vector<std::string> cid_vec;
  for (size_t i = 0; i < children_.size(); ++i) {
    std::string cid_str = children_[i].first.ToString();
    auto cid = package_node.add_cid();
    *cid = cid_str;
  }
  return std::make_unique<Node>(package_node);
}

}  // namespace linked_data
}  // namespace cognitio
