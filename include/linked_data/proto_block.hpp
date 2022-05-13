// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_BLOCK_HPP_
#define CGNT_LINKED_DATA_BLOCK_HPP_

#include <vector>

#include "common/multiformats/cid.hpp"
#include "linked_data/dag_node.hpp"
#include "proto/data/ProtoData.pb.h"

namespace cognitio {
namespace linked_data {

/**
 *  @brief  Special wrapper for proto Block message.
 *
 *  Use to make and read from Block messages.
 */
class ProtoBlock {
  typedef common::Cid Cid;
  typedef std::unique_ptr<Block> ProtoBlockPtr;

 public:
  ProtoBlock() noexcept = default;
  ~ProtoBlock() noexcept = default;

  /**
   *  @brief  Block constructor.
   *
   *  @param bytes to save in member. May be empty.
   *  @param cid to save in member.
   */
  ProtoBlock(const Cid &cid, const DagNode &node = DagNode()) noexcept
      : cid_(cid), node_(node) {}

  /**
   *  @brief  Convert block to proto Block message.
   *
   *  @return shared ptr on proto Block class.
   */
  ProtoBlockPtr ToProtoMessage();

  /**
   *  @brief  Read from proto Block message.
   *
   *  @param proto_block message.
   */
  Status FromProtoMessage(const ProtoBlockPtr &proto_block);

  /**
   *  @brief  Get node from Block.
   *
   *  @return vector of bytes.
   */
  DagNode GetNode() const noexcept { return node_; }

  /**
   *  @brief  Set node to Block.
   *
   *  @param bytes to store in Block.
   */
  void SetNode(const DagNode &node) noexcept { node_ = node; }

  /**
   *  @brief  Get cid from Block.
   *
   *  @return Cid.
   */
  Cid GetCid() const noexcept { return cid_; }

  /**
   *  @brief  Set cid to Block.
   *
   *  @param cid to store in Block.
   */
  void SetCid(const Cid &cid) { cid_ = cid; }

  bool IsInitialized() { return cid_; }

 private:
  Cid cid_;
  DagNode node_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_BLOCK_HPP_
