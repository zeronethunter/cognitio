//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_EXCHANGE_BLOCK_SWAP_RPC_SERVICE_HPP_
#define CGNT_EXCHANGE_BLOCK_SWAP_RPC_SERVICE_HPP_

#include <grpc++/grpc++.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include "proto/ProtoData.grpc.pb.h"
#include "proto/ProtoData.pb.h"
#include "repo/repo.hpp"

namespace cognitio {
namespace exchange {

class BlockSwapServiceImpl final : public BlockSwapService::Service {
 public:
  typedef std::shared_ptr<repo::Repo<std::string>> RepoPtr;

  BlockSwapServiceImpl(RepoPtr repo) : repo_(repo) {}
  ~BlockSwapServiceImpl() = default;

  grpc::Status GetBlock(::grpc::ServerContext* context,
                        const GetBlockRequest* req,
                        GetBlockResponse* resp) override;

 private:
  RepoPtr repo_;
};

}  // namespace exchange
}  // namespace cognitio

#endif  // CGNT_EXCHANGE_BLOCK_SWAP_RPC_SERVICE_HPP_
