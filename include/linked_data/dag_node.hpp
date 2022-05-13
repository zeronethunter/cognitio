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
#include "common/multiformats/cid.hpp"
#include "proto/link/ProtoLink.pb.h"

namespace cognitio {
namespace linked_data {

template <typename Data>
class DagNode {
 public:
  DagNode() = default;
  explicit DagNode(Data &&data) : content_(std::move(data)) {};

  DagNode(DagNode &&other) noexcept;
  DagNode &operator=(DagNode &&other) noexcept;

  /// \return content of node
  Data GetContent() const;

  /// \return size of node's children
  size_t Count() const;

  common::Cid GetCid() const;

  std::unique_ptr<DagNode> GetSubNode(std::string_view name) const;

  std::vector<std::string_view> GetSubNodeNames() const;

  /// \brief insert children
  Status InsertSubNode(std::string &&name, DagNode &&children_node);

 private:
  std::vector<uint8_t> content_;
  std::map<std::string, DagNode, std::less<> > children_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_NODE_HPP
