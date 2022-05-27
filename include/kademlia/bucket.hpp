//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_BUCKET_HPP_
#define CGNT_KADEMLIA_BUCKET_HPP_

#include <boost/unordered_map.hpp>
#include <mutex>
#include <vector>

#include "kademlia/connection_info.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/sys_def.hpp"

namespace cognitio {
namespace kademlia {

class Bucket {
 public:
  Bucket() = default;
  Bucket(const Identifier& id) : id_(id) {}
  ~Bucket() = default;

  Identifier getId() const;
  Identifier::numeric_type size() const;

  bool contains(const Identifier& id) const;
  bool contains(const ConnectionInfo& connection_info) const;

  void add(const ConnectionInfo& connection_info);
  void remove(const Identifier& id);
  void remove(const ConnectionInfo& connection_info);

  void pushToFront(const Identifier& id);

  ConnectionInfo get(const Identifier& id);
  const std::vector<Identifier>& getNodeIds() const;

  std::string toString() const;

 private:
  Identifier id_;
  std::vector<Identifier> node_ids_;
  boost::unordered_map<Identifier::numeric_type, ConnectionInfo> nodes_map_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_BUCKET_HPP_
