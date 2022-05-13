// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_DAG_NODE_HPP_
#define CGNT_LINKED_DATA_DAG_NODE_HPP_

#include <map>
#include <string>
#include <vector>

#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "proto/data/ProtoData.pb.h"
#include "files/unixfs/unixfs.hpp"

namespace cognitio {
namespace linked_data {

class DagNode {
 public:
  DagNode() = default;

  explicit DagNode(files::unixfs::UnixFS &&file) : data_(std::move(file)) {};
  DagNode &operator=(files::unixfs::UnixFS &&file);

  explicit DagNode(std::vector<uint8_t> &&bytes) {
    data_.SetData(std::move(bytes));
  }
  DagNode &operator=(std::vector<uint8_t> &&bytes);

  /// \return content of node
  std::vector<uint8_t> GetContent() const;

  Status DecodeProtoNode(const Node &node);
  std::unique_ptr<Node> EncodeProtoNode() const;

  /// \return size of node's children
  size_t Count() const;

  common::Cid GetCid() const;

  std::unique_ptr<DagNode> GetSubNode(std::string_view name) const;

  std::vector<common::Cid> GetSubNodeNames() const;

  /// \brief insert children
  Status InsertSubNode(std::string &&name, DagNode &&children_node);

 private:
  files::unixfs::UnixFS data_;
  std::map<common::Cid, DagNode, std::less<>> children_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_DAG_NODE_HPP_
