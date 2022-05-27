//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_KADEMLIA_HPP_
#define CGNT_KADEMLIA_KADEMLIA_HPP_

#include <cstdint>
#include <thread>

#include "common/status.hpp"
#include "kademlia/connection_info.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/routing_table.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace kademlia {

class Kademlia {
 public:
  explicit Kademlia(Identifier id, std::string& address, uint32_t port);

  void Run() noexcept;
  void Shutdown() noexcept;

  Identifier GetId() const noexcept;
  ConnectionInfo GetAddress() const noexcept;
  RoutingTable& GetRoutingTable() noexcept;
  bool IsAlive() const noexcept;

  void Bootstrap(ConnectionInfo bootstrap_node);
  Status Ping(const ConnectionInfo& id);
  Status Add(std::string key, std::string value);
  std::string Get(std::string key);

  ~Kademlia();

 private:
  void mainThread() noexcept;
  void refreshNodes() noexcept;
  void pingAndUpdate(std::vector<ConnectionInfo>& targets) noexcept;
  FindNodeAnswer findNode(ConnectionInfo& caller, ConnectionInfo& node,
                          Identifier &destination);

  RoutingTable routing_table_;
  std::atomic<bool> alive_;
  ConnectionInfo info_;

  std::thread main_thread_;
  std::vector<ConnectionInfo> reference_nodes_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_KADEMLIA_HPP_
