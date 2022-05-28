//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/rpc/service.hpp"

#include "kademlia/connection_info.hpp"
#include "kademlia/kademlia.hpp"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

::grpc::Status KademliaServiceImpl::Ping(
    [[maybe_unused]] ::grpc::ServerContext* context, const PingRequest* request,
    PingResponse* response) {
  response->set_magic(request->magic());
  ConnectionInfo callerInfo(Identifier::FromString(request->caller().id()),
                            request->caller().ip(), request->caller().port());

  dht_->GetRoutingTable().update(callerInfo);
  return ::grpc::Status::OK;
}

::grpc::Status KademliaServiceImpl::FindNode(
    [[maybe_unused]] ::grpc::ServerContext* context,
    const FindNodeRequest* request, FindNodeResponse* response) {
  logger_->debug("Recieved FindNode request");

  try {
    FindNodeAnswer result = dht_->GetRoutingTable().findClosest(
        Identifier::FromString(request->destination()));

    for (auto peer : result) {
      auto conn_info = response->add_closest();
      conn_info->set_id((std::string)peer.id());
      conn_info->set_ip(peer.ip());
      conn_info->set_port(peer.port());
    }

  } catch (std::exception& e) {
    return grpc::Status(grpc::StatusCode::INTERNAL, e.what());
  }

  return ::grpc::Status::OK;
}

::grpc::Status KademliaServiceImpl::Store(
    [[maybe_unused]] ::grpc::ServerContext* context,
    const StoreRequest* request, StoreResponse* response) {
  logger_->debug("Recieved Store request");
  response->set_magic(request->magic());
  dht_->StoreRequested(request->key(), request->value());
  return ::grpc::Status::OK;
}

::grpc::Status KademliaServiceImpl::Get(
    [[maybe_unused]] ::grpc::ServerContext* context, const GetRequest* request,
    GetResponse* response) {
  logger_->debug("Recieved get request");

  try {
    response->set_magic(request->magic());
    response->set_value(dht_->GetRequested(request->key()));

  } catch (...) {
    logger_->info("Not found requested key");
    return ::grpc::Status(grpc::StatusCode::NOT_FOUND, "Key not found");
  }

  return ::grpc::Status::OK;
}

}  // namespace kademlia
}  // namespace cognitio
