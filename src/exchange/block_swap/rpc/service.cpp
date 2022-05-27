//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "exchange/block_swap/rpc/service.hpp"

#include <grpcpp/support/status.h>
#include "multiformats/cid.hpp"

namespace cognitio {
namespace exchange {

grpc::Status BlockSwapServiceImpl::GetBlock(::grpc::ServerContext *context,
                                            const GetBlockRequest *req,
                                            GetBlockResponse *resp) {
  // TODO:
  auto block = repo_->Get(common::Cid(req->cid()));
}

}  // namespace exchange
}  // namespace cognitio
