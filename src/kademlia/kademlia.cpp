//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/kademlia.hpp"

#include <grpcpp/grpcpp.h>

#include <thread>

#include "common/status.hpp"
#include "kademlia/connection_info.hpp"
#include "kademlia/routing_table.hpp"
#include "kademlia/rpc/client.hpp"
#include "kademlia/utils.hpp"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

Kademlia::Kademlia(ConnectionInfo& my_node)
    : repo_(my_node.id()),
      routing_table_(my_node.id()),
      alive_(false),
      info_(my_node) {}

void Kademlia::Shutdown() noexcept {
  alive_ = false;
  if (main_thread_.joinable()) {
    main_thread_.join();
  }
}

Kademlia::~Kademlia() {
  if (IsAlive()) {
    Shutdown();
  }
}

Identifier Kademlia::GetId() const noexcept { return info_.id(); }
ConnectionInfo Kademlia::GetAddress() const noexcept { return info_; }
RoutingTable& Kademlia::GetRoutingTable() noexcept { return routing_table_; }
bool Kademlia::IsAlive() const noexcept { return alive_; }

void Kademlia::Run() noexcept {
  alive_ = true;
  main_thread_ = std::thread(&Kademlia::mainThread, this);
}

void Kademlia::pingAndUpdate(std::vector<ConnectionInfo>& targets) noexcept {
  for (auto peer : targets) {
    KademliaClient client(peer.GetAddress());
    PingRequest pingQuery;

    auto proto = info_.GetProto();
    pingQuery.set_magic(utils::generate_magic());
    pingQuery.set_allocated_caller(&proto);

    if (client.Ping(pingQuery)) {
      routing_table_.update(peer);
    } else {
      routing_table_.remove(peer);
    }

    [[maybe_unused]] auto v = pingQuery.release_caller();
  }
}

void Kademlia::mainThread() noexcept {
  while (IsAlive()) {
    refreshNodes();
    pingAndUpdate(reference_nodes_);

    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point end;

    do {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(1s);
      end = std::chrono::steady_clock::now();
      if (!IsAlive()) {
        break;
      }
    } while (std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
                 .count() < PROCESS_MILLISECONDS_INTERVAL);
  }
}

Status Kademlia::Ping(const ConnectionInfo& id) {
  KademliaClient client(id.GetAddress());
  PingRequest query;
  query.set_magic(utils::generate_magic());
  auto is_ok = client.Ping(query);
  return is_ok ? Status::OK : Status::FAILED;
}

FindNodeAnswer Kademlia::findNode(ConnectionInfo& caller, ConnectionInfo& node,
                                  Identifier& destination) {
  if (node.id() != info_.id()) {
    routing_table_.update(node);
  }

  FindNodeAnswer closestPeers = routing_table_.findClosest(destination);
  FindNodeAnswer prevClosestPeers;

  while (!closestPeers.found() && prevClosestPeers != closestPeers) {
    prevClosestPeers = closestPeers;

    FindNodeAnswer postSearch(destination);
    for (auto peer : closestPeers) {
      KademliaClient client(peer.GetAddress());

      FindNodeRequest findQuery;
      auto proto = caller.GetProto();
      findQuery.set_allocated_caller(&proto);
      findQuery.set_destination(destination);
      postSearch.merge(client.FindNode(findQuery));
      [[maybe_unused]] auto v = findQuery.release_caller();
    }

    closestPeers.merge(postSearch);
  }

  return closestPeers;
}

Status Kademlia::Add(std::string key, std::string value) {
  Identifier keyHash =
      utils::hash_key(key, GUID_SPACE).convert_to<Identifier::numeric_type>();

  // Find the target node
  FindNodeAnswer findResult = findNode(info_, info_, keyHash);

  if (findResult.size() < 1) {
    return Status::FAILED;
  }

  // ConnectionInfo peer = findResult.getNodes()[0];
  // KademliaClient client(peer.GetAddress());

  // StoreRequest storeQuery;
  // storeQuery.set_key(key);
  // storeQuery.set_value(value);
  // auto proto = info_.GetProto();
  // storeQuery.set_magic(utils::generate_magic());
  // storeQuery.set_allocated_caller(&proto);

  // auto is_ok = client.Store(storeQuery);
  // [[maybe_unused]] auto v = storeQuery.release_caller();
  // return is_ok ? Status::OK : Status::FAILED;

  auto peers = findResult.getNodes();
  for (size_t i = 0; i < KADEMLIA_REPLICATION_NUMBER && i < peers.size(); ++i) {
    ConnectionInfo peer = findResult.getNodes()[i];
    KademliaClient client(peer.GetAddress());

    StoreRequest storeQuery;
    storeQuery.set_key(key);
    storeQuery.set_value(value);
    auto proto = info_.GetProto();
    storeQuery.set_magic(utils::generate_magic());
    storeQuery.set_allocated_caller(&proto);

    client.Store(storeQuery);
    [[maybe_unused]] auto v = storeQuery.release_caller();
  }

  return Status::OK;
}

void Kademlia::Bootstrap(ConnectionInfo bootstrap_node) {
  auto my_id = info_.id();
  FindNodeAnswer findResult = findNode(info_, bootstrap_node, my_id);
  routing_table_.update(bootstrap_node);
  auto nodes = findResult.getNodes();
  pingAndUpdate(nodes);
}

std::string Kademlia::Get(std::string key) {
  if (repo_.contains(key)) {
    return repo_.get(key);
  }

  Identifier keyHash =
      utils::hash_key(key, GUID_SPACE).convert_to<Identifier::numeric_type>();

  // Find the target node
  FindNodeAnswer findResult = this->findNode(info_, info_, keyHash);

  if (findResult.size() < 1) {
    // throw std::runtime_error("E_NO_PEERS");
    return "";
  }

  ConnectionInfo peer = findResult.getNodes()[0];
  KademliaClient client(peer.GetAddress());

  GetRequest getQuery;
  auto proto = info_.GetProto();
  getQuery.set_key(key);
  getQuery.set_magic(utils::generate_magic());
  getQuery.set_allocated_caller(&proto);

  auto to_ret = client.Get(getQuery);

  [[maybe_unused]] auto v = getQuery.release_caller();
  return to_ret;
}

void Kademlia::StoreRequested(const std::string& key,
                              const std::string& value) {
  repo_.store(key, value);
}

// may throw exception
std::string Kademlia::GetRequested(const std::string& key) {
  return repo_.get(key);
}

void Kademlia::refreshNodes() noexcept {
  reference_nodes_.clear();
  std::vector<Identifier> distances =
      Identifier::GetDirectReferenceIds(info_.id());

  for (const auto& d : distances) {
    FindNodeAnswer findNodeAnswer = routing_table_.findClosest(d);
    if (findNodeAnswer.size() > 0) {
      reference_nodes_.push_back(findNodeAnswer.getNodes()[0]);
    }
  }
}

}  // namespace kademlia
}  // namespace cognitio
