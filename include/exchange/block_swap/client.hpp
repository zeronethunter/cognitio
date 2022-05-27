//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_EXCHANGE_BLOCK_SWAP_CLIENT_HPP_
#define CGNT_EXCHANGE_BLOCK_SWAP_CLIENT_HPP_

#include "grpc_wrapper/client/client.hpp"
#include "linked_data/proto_block.hpp"
#include "proto/ProtoData.grpc.pb.h"

namespace cognitio {
namespace exchange {

class BlockSwapClient {
 public:
  BlockSwapClient(std::string address) : client_(address) {}
  linked_data::ProtoBlock GetNode(const std::string &cid);
  ~BlockSwapClient() = default;

 private:
  rpc::client::Client<BlockSwapService> client_;
};

}  // namespace exchange
}  // namespace cognitio

#endif  // CGNT_EXCHANGE_BLOCK_SWAP_CLIENT_HPP_
