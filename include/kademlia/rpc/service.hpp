//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_SERVICE_HPP_
#define CGNT_KADEMLIA_SERVICE_HPP_

#include <grpc++/grpc++.h>

#include "common/logger/logger.hpp"
#include "exchange/block_swap/block_swap.hpp"
#include "kademlia/kademlia.hpp"
#include "proto/kademlia.grpc.pb.h"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

class KademliaServiceImpl final : public KademliaService::Service {
 public:
  typedef std::shared_ptr<Kademlia> KadPtr;

  KademliaServiceImpl(KadPtr kad) : dht_(kad) {
    logger_ = common::logger::createLogger("kademlia_service");
  }

  ~KademliaServiceImpl() = default;

  grpc::Status Ping(::grpc::ServerContext* context, const PingRequest* request,
                    PingResponse* response) override;

  grpc::Status FindNode(::grpc::ServerContext* context,
                        const FindNodeRequest* request,
                        FindNodeResponse* response) override;

  grpc::Status Store(::grpc::ServerContext* context,
                     const StoreRequest* request,
                     StoreResponse* response) override;

  grpc::Status Get(::grpc::ServerContext* context, const GetRequest* request,
                   GetResponse* response) override;

 private:
  common::logger::Logger logger_;
  KadPtr dht_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_SERVICE_HPP_
