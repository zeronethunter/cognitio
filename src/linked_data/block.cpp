#include "linked_data/block.hpp"

namespace cognitio {
namespace linked_data {

ProtoBlock Block::ToProtoMessage() {
  ::Block new_block;
  std::unique_ptr<Node> new_node = node_.EncodeProtoNode();

  new_block.set_cid(cid_.String());
  new_block.set_allocated_node(new_node.get());

  return std::make_unique<::Block>(new_block);
}

Status Block::FromProtoMessage(const ProtoBlock &proto_block) {
  if (proto_block->IsInitialized()) {
    cid_ = Cid(proto_block->cid());
    node_.DecodeProtoNode(proto_block->node());
    return Status::OK;
  }
  return Status(StatusCode::FAILED, "Block message is not initialized.");
}

}  // namespace linked_data
}  // namespace cognitio