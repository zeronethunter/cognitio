// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_DAG_NODE_HPP_
#define CGNT_LINKED_DATA_DAG_NODE_HPP_

#include <string>
#include <vector>

#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "files/unixfs/unixfs.hpp"
#include "proto/ProtoData.pb.h"

namespace cognitio {
namespace linked_data {

class DagNode {
 public:
  DagNode() = default;

  DagNode(const DagNode &other) = default;
  DagNode &operator=(const DagNode &other) = default;

  explicit DagNode(files::unixfs::UnixFS &&file) : data_(std::move(file)){};
  DagNode &operator=(files::unixfs::UnixFS &&file);

  explicit DagNode(std::vector<uint8_t> &&bytes) {
    data_.SetData(bytes);
  }
  DagNode &operator=(std::vector<uint8_t> &&bytes);

  DagNode(std::vector<uint8_t> &&bytes,
          const std::vector<DagNode> &children_data);

  /// \return bytes of node
  std::vector<uint8_t> GetContent() const;

  Status DecodeProtoNode(const Node &node);
  std::unique_ptr<Node> EncodeProtoNode() const;

  /// \return size of node's children
  size_t Count() const;

  /// \return cid of current node
  common::Cid GetCid() const;

  /// \brief insert children
  // Status InsertSubNode(std::string &&name, DagNode &&children_node);

  const std::vector<std::pair<common::Cid, std::shared_ptr<DagNode>>>
      &GetChildren() const {
    return children_;
  };

 private:
  files::unixfs::UnixFS data_;
  std::vector<std::pair<common::Cid, std::shared_ptr<DagNode>>> children_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_DAG_NODE_HPP_
