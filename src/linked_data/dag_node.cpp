// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/dag_node.hpp"

#include <span>

namespace cognitio {
namespace linked_data {

std::vector<uint8_t> DagNode::GetContent() const { return content_; }

size_t DagNode::Count() const { return children_.size(); }

std::unique_ptr<DagNode> DagNode::GetSubNode(std::string_view name) const {
  if (auto iter = children_.find(name); iter != children_.end()) {
    return std::make_unique<DagNode>(iter->second);
  }
}

std::vector<std::string_view> DagNode::GetSubNodeNames() const {
  std::vector<std::string_view> names_vec;
  for (const auto &element : children_) {
    names_vec.push_back(element.first);
  }
  return names_vec;
}

common::Cid DagNode::GetCid() const {
  return common::Cid(content_);
}

Status DagNode::InsertSubNode(std::string &&name, DagNode &&children_node) {
  auto result = children_.emplace(std::move(name), std::move(children_node));
  if (result.second) {
    return Status::OK;
  }
  return Status(CANCELLED, "Duplicate node");
}

}  // namespace linked_data
}  // namespace cognitio
