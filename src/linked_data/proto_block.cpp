#include "linked_data/proto_block.hpp"

namespace cognitio {
namespace linked_data {

std::unique_ptr<Block> ProtoBlock::ToProtoMessage() const noexcept {
  Block new_block;
  std::unique_ptr<Node> new_node = node_.EncodeProtoNode();

  new_block.set_cid(cid_.ToString());
  new_block.set_allocated_node(new_node.get());

  new_node.release();

  return std::make_unique<Block>(new_block);
}

Status ProtoBlock::FromProtoMessage(
    const std::unique_ptr<Block> &proto_block) noexcept {
  if (proto_block->IsInitialized()) {
    cid_ = Cid(proto_block->cid());
    node_.DecodeProtoNode(proto_block->node());
    return Status::OK;
  }
  return {StatusCode::FAILED, "Block message is not initialized."};
}

}  // namespace linked_data
}  // namespace cognitio
