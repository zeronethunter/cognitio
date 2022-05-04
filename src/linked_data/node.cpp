// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/node.hpp"

namespace cognitio {
namespace linked_data {

std::vector<uint8_t> Node::GetContent() const { return content_; }

size_t Node::Count() const { return children_.size(); }

std::unique_ptr<Node> Node::GetSubNode(std::string_view name) const {
  if (auto iter = children_.find(name); iter != children_.end()) {
    return std::make_unique<Node>(iter->second);
  }
}

std::vector<std::string_view> Node::GetSubNodeNames() const {
  std::vector<std::string_view> names_vec;
  for (const auto &element : children_) {
    names_vec.push_back(element.first);
  }
  return names_vec;
}

Status Node::InsertSubNode(std::string &&name, Node &&children_node) {
    auto result = children_.emplace(std::move(name), std::move(children_node));
    if (result.second) {
        return Status::OK;
    }
    return Status(CANCELLED, "Duplicate node");
}

}  // namespace linked_data
}  // namespace cognitio
