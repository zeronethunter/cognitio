// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <vector>

#include "common/multiformats/cid.hpp"
#include "linked_data/node.hpp"

#ifndef CGNT_LINKED_DATA_MERKLE_DAG_HPP
#define CGNT_LINKED_DATA_MERKLE_DAG_HPP

namespace cognitio {
namespace linked_data {

class BlockService {};

//! Directed Acyclic Graph
class MerkleDag 
{
 public:
  MerkleDag() = default;

  //! Creates new MerkleDag on blocks source
  MerkleDag(const BlockService &blocks);

  //! Adds new Node in Dag
  void AddNode(const cognitio::linked_data::Node &node);

  //! Gets Node by Cid
  cognitio::linked_data::Node GetNode(const cognitio::cid::Cid &cid) const;

  //! Removes Node from Dag by Cid
  void RemoveNode(const cognitio::cid::Cid &cid);

  //! Getting array of Nodes by Directed Traversal Dag
  std::vector<cognitio::linked_data::Node> DirectedTrasersal() const;

 private:
  std::vector<cognitio::linked_data::Node> dag_view;  //! tree in vec view
  BlockService blocks_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_MERKLE_DAG_HPP
