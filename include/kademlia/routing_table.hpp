//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_ROUTING_TABLE_HPP_
#define CGNT_KADEMLIA_ROUTING_TABLE_HPP_

#include <mutex>
#include <vector>

#include "kademlia/bucket.hpp"
#include "kademlia/connection_info.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/models/find_node_answ.hpp"

namespace cognitio {
namespace kademlia {

class RoutingTable {
 public:
  RoutingTable(const Identifier& ownerID);
  ~RoutingTable() = default;

  void update(const ConnectionInfo& connection_info);
  void remove(const ConnectionInfo& connection_info);
  FindNodeAnswer findClosest(const Identifier& destinationId);
  std::string toString() const;

 private:
  Identifier ownerId_;
  std::vector<Bucket> buckets_;
  std::mutex locker_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_ROUTING_TABLE_HPP_
