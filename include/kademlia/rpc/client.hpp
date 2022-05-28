//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_RPC_CLIENT_HPP_
#define CGNT_KADEMLIA_RPC_CLIENT_HPP_

#include <google/protobuf/stubs/logging.h>
#include <grpc++/grpc++.h>

#include "common/logger/logger.hpp"
#include "grpc_wrapper/client/client.hpp"
#include "proto/kademlia.grpc.pb.h"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

class KademliaClient {
 public:
  KademliaClient(std::string address) : client_(address) {
    logger_ = common::logger::createLogger("kademlia_client");
  }

  bool Ping(PingRequest query);
  class FindNodeAnswer FindNode(FindNodeRequest query);
  bool Store(StoreRequest query);
  std::string Get(GetRequest query);

 private:
  common::logger::Logger logger_;
  rpc::client::Client<KademliaService> client_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_RPC_CLIENT_HPP_
