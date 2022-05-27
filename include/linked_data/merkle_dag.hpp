// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <functional>
#include <memory>
#include <vector>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "exchange/block_service/block_service.hpp"
#include "linked_data/dag_node.hpp"
#include "linked_data/proto_block.hpp"
#include "multiformats/cid.hpp"

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
  explicit MerkleDag(std::shared_ptr<exchange::BlockService> block_service)
      : logger_(common::logger::createLogger("Merkle Dag")),
        block_service_(block_service),
        merkle_root_(nullptr){};

  /// \brief Adds new Node in Dag
  Status AddNode(const DagNode &node);

  std::pair<Status, common::Cid> Add(
      const std::vector<std::vector<uint8_t>> &chunks);

  /// \brief Gets Node by Cid
  std::pair<Status, DagNode> GetNode(const common::Cid &cid) const;

  /// \brief gets name of file by Cid
  std::pair<Status, std::vector<uint8_t>> Get(const common::Cid &cid) const;

  /// \brief Removes Node from Dag by Cid
  Status RemoveNode(const common::Cid &cid, bool is_recursive = true);

  /// \brief Getting array of Blocks by Directed Traversal Dag
  std::vector<ProtoBlock> CollectBlocks(const DagNode &node) const;

  /// \brief Getting array of Nodes by Directed Traversal Dag
  std::vector<DagNode> CollectNodes(const DagNode &root_node) const;

 private:
  common::logger::Logger logger_;

  std::shared_ptr<exchange::BlockService> block_service_ = nullptr;
  std::unique_ptr<DagNode> merkle_root_ = nullptr;

  std::unique_ptr<DagNode> buildGraph(
      const std::vector<std::vector<uint8_t>> &chunks);

  std::vector<DagNode> getSubNodes(const DagNode &root) const;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_MERKLE_DAG_HPP
