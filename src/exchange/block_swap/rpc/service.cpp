//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "exchange/block_swap/rpc/service.hpp"

#include <grpcpp/support/status.h>

#include "linked_data/proto_block.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace exchange {

grpc::Status BlockSwapServiceImpl::GetBlock(
    [[maybe_unused]] ::grpc::ServerContext *context, const GetBlockRequest *req,
    GetBlockResponse *resp) {
  logger_->debug("Recieved get block request");
  linked_data::ProtoBlock block;

  auto cid = common::Cid(req->cid());
  block = repo_->Get(common::Cid(cid));

  if (block.IsInitialized()) {
    logger_->debug("Found needed block");
    auto proto = block.ToProtoMessage();
    resp->set_allocated_block(proto.get());
    resp->set_ok(true);
    [[maybe_unused]] auto v = proto.release();
  } else {
    logger_->debug("Needed block not found");
    resp->set_ok(false);
  }

  return grpc::Status::OK;
}

}  // namespace exchange
}  // namespace cognitio
