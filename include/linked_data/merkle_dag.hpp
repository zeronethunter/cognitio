// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <memory>
#include <vector>

#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "linked_data/node.hpp"
#include "repo/block_storage/block_storage.hpp"

#ifndef CGNT_LINKED_DATA_MERKLE_DAG_HPP
#define CGNT_LINKED_DATA_MERKLE_DAG_HPP

namespace cognitio {
namespace linked_data {

//! Directed Acyclic Graph
class MerkleDag {
 public:
  MerkleDag() = default;

  //! Creates new MerkleDag on blocks source
  MerkleDag(const repo::blockstorage::Blockstorage &blocks);

  //! Adds new Node in Dag
  Status AddNode(const cognitio::linked_data::Node &node);

  //! Gets Node by Cid
  std::pair<Status, cognitio::linked_data::Node> GetNode(const cognitio::common::Cid &cid) const;

  //! Removes Node from Dag by Cid
  Status RemoveNode(const cognitio::common::Cid &cid);

  //! Getting array of Nodes by Directed Traversal Dag
  std::vector<cognitio::linked_data::Node> DirectedTrasersal() const;

 private:
  std::shared_ptr<repo::blockstorage::Blockstorage> block_service_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_MERKLE_DAG_HPP
