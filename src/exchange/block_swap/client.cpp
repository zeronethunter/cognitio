//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "exchange/block_swap/client.hpp"

#include <grpc++/grpc++.h>

#include "linked_data/proto_block.hpp"
#include "proto/ProtoData.pb.h"

namespace cognitio {
namespace exchange {

linked_data::ProtoBlock BlockSwapClient::GetNode(const std::string& cid) {
  auto stub = client_.GetStub();
  grpc::ClientContext context;

  GetBlockRequest req;
  req.set_cid(cid);

  GetBlockResponse resp;
  auto err = stub->GetBlock(&context, req, &resp);
  if (err.ok()) {
    linked_data::ProtoBlock block;
    block.FromProtoMessage(std::make_unique<::Block>(resp.block()));
    return block;
  }

  return linked_data::ProtoBlock();
}

}  // namespace exchange
}  // namespace cognitio
