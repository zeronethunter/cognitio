// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <functional>
#include <memory>
#include <vector>

#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "exchange/block_service/block_service.hpp"
#include "linked_data/dag_node.hpp"
#include "linked_data/link.hpp"
#include "linked_data/node.hpp"

#ifndef CGNT_LINKED_DATA_MERKLE_DAG_HPP
#define CGNT_LINKED_DATA_MERKLE_DAG_HPP

#define CHUNK_SIZE 10

namespace cognitio {
namespace linked_data {

/// Directed Acyclic Graph
class MerkleDag {
 public:
  /// \brief creates new MerkleDag on blocks source
  explicit MerkleDag(std::shared_ptr<exchange::BlockService> block_service);

  /// \brief Adds new Node in Dag
  Status AddNode(const DagNode &node);

  std::pair<Status, common::Cid> Add(
      const std::vector<std::vector<uint8_t>> &chunks);

  /// \brief Gets Node by Cid
  std::pair<Status, DagNode> GetNode(const common::Cid &cid) const;

  /// \brief gets name of file by Cid
  std::string Get(const common::Cid &cid);

  /// \brief Removes Node from Dag by Cid
  Status RemoveNode(const common::Cid &cid, bool is_recursive = true);

  /// \brief Getting array of Nodes by Directed Traversal Dag
  std::vector<DagNode> DirectedTrasersal(DagNode node) const;

 private:
  std::shared_ptr<exchange::BlockService> block_service_;
  std::unique_ptr<DagNode> merkle_root_;

  std::unique_ptr<DagNode> buildGraph(const std::vector<std::vector<uint8_t>> &chunks);
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_MERKLE_DAG_HPP
