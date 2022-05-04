// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_NODE_HPP
#define CGNT_LINKED_DATA_NODE_HPP

#include <map>
#include <string>
#include <vector>

#include "common/status.hpp"

namespace cognitio {
namespace linked_data {

class Node {
 public:
  explicit Node(std::vector<uint8_t> &&bytes) : content_(std::move(bytes)) {};

  /// \return content of node
  std::vector<uint8_t> GetContent() const;

  /// \return size of node's children
  size_t Count() const;

  std::unique_ptr<Node> GetSubNode(std::string_view name) const;

  std::vector<std::string_view> GetSubNodeNames() const;

  /// \brief insert children
  Status InsertSubNode(std::string &&name, Node &&children_node);

 private:
  std::vector<uint8_t> content_;
  std::map<std::string, Node, std::less<> > children_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_NODE_HPP
