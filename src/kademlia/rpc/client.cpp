//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/rpc/client.hpp"

#include "kademlia/models/find_node_answ.hpp"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

bool KademliaClient::Ping(PingRequest query) {
  auto stub = client_.GetStub();
  grpc::ClientContext context;
  PingResponse reply;

  if (!stub->Ping(&context, query, &reply).ok()) {
    return false;
  }

  return query.magic() == reply.magic();
}

FindNodeAnswer KademliaClient::FindNode(FindNodeRequest query) {
  auto stub = client_.GetStub();
  grpc::ClientContext context;
  FindNodeResponse reply;

  grpc::Status result = stub->FindNode(&context, query, &reply);

  if (!result.ok()) {
    std::cout << result.error_code() << ": " << result.error_message()
              << std::endl;
    return FindNodeAnswer();
  }

  FindNodeAnswer answer(Identifier::FromString(query.destination()));

  for (int i = 0; i < reply.closest_size(); i++) {
    ::ConnectionInfo info = reply.closest().Get(i);
    answer.add(ConnectionInfo(Identifier::FromString(info.id()), info.ip(),
                              info.port()));
  }

  return answer;
}

bool KademliaClient::Store(StoreRequest query) {
  auto stub = client_.GetStub();
  grpc::ClientContext context;
  StoreResponse reply;

  if (!stub->Store(&context, query, &reply).ok()) {
    return false;
  }

  return query.magic() == reply.magic();
}

std::string KademliaClient::Get(GetRequest query) {
  auto stub = client_.GetStub();
  grpc::ClientContext context;
  GetResponse reply;

  grpc::Status result = stub->Get(&context, query, &reply);
  if (!result.ok()) {
    throw result.error_message();
  }

  return reply.value();
}

}  // namespace kademlia
}  // namespace cognitio
