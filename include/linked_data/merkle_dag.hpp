// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <functional>
#include <memory>
#include <vector>

#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "linked_data/dag_node.hpp"
#include "linked_data/link.hpp"
#include "linked_data/node.hpp"
#include "repo/block_storage/block_storage.hpp"

#ifndef CGNT_LINKED_DATA_MERKLE_DAG_HPP
#define CGNT_LINKED_DATA_MERKLE_DAG_HPP

#define CHUNK_SIZE 10

namespace cognitio {
namespace linked_data {

/// Directed Acyclic Graph
class MerkleDag {
 public:
  MerkleDag() = default;

  /// \brief creates new MerkleDag on blocks source
  MerkleDag(const repo::blockstorage::Blockstorage &blocks);

  /// \brief Adds new Node in Dag
  Status AddNode(const DagNode &node);

  /// \brief Gets Node by Cid
  std::pair<Status, DagNode> GetNode(
      const common::Cid &cid) const;

  /// \brief gets name of file by Cid
  std::string Get(const common::Cid &cid);

  /// \brief Removes Node from Dag by Cid
  Status RemoveNode(const common::Cid &cid);

  /// \brief Getting array of Nodes by Directed Traversal Dag
  std::vector<cognitio::linked_data::DagNode> DirectedTrasersal() const;

 private:
  std::shared_ptr<repo::blockstorage::Blockstorage> block_service_;

  Status buildGraph(const std::vector<std::vector<uint8_t>> &chunks);
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_MERKLE_DAG_HPP
