//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/bucket.hpp"

namespace cognitio {
namespace kademlia {

Identifier Bucket::getId() const { return id_; }

Identifier::numeric_type Bucket::size() const {
  return (Identifier::numeric_type)node_ids_.size();
}

bool Bucket::contains(const Identifier& id) const {
  for (size_t i = 0; i < node_ids_.size(); ++i) {
    if (node_ids_[i] == id) {
      return true;
    }
  }

  return false;
}

bool Bucket::contains(const ConnectionInfo& connection_info) const {
  return contains(connection_info.id());
}

void Bucket::add(const ConnectionInfo& connection_info) {
  // Remove the element if already inserted
  remove(connection_info.id());

  // insert the new id on the nodes ids list
  node_ids_.insert(node_ids_.begin(), connection_info.id());

  // insert the new id on the map
  nodes_map_[connection_info.id()] = connection_info;
}

void Bucket::remove(const Identifier& id) {
  std::vector<Identifier>::iterator ids_iterator = std::find_if(
      node_ids_.begin(), node_ids_.end(),
      [id](const Identifier& element) -> bool { return id == element; });

  if (ids_iterator != node_ids_.end()) {
    node_ids_.erase(ids_iterator);
    nodes_map_.erase(id);
  }
}

void Bucket::remove(const ConnectionInfo& connection_info) {
  remove(connection_info.id());
}

void Bucket::pushToFront(const Identifier& id) {
  std::vector<Identifier>::iterator ids_iterator = std::find_if(
      node_ids_.begin(), node_ids_.end(),
      [id](const Identifier& element) -> bool { return id == element; });

  if (ids_iterator != node_ids_.end()) {
    node_ids_.erase(ids_iterator);
    node_ids_.insert(node_ids_.begin(), id);
  }
}

ConnectionInfo Bucket::get(const Identifier& id) {
  if (!contains(id)) {
    throw std::runtime_error("E_UNKNOWN_ID");
  }
  return nodes_map_[id];
}

const std::vector<Identifier>& Bucket::getNodeIds() const { return node_ids_; }

std::string Bucket::toString() const {
  std::string result;

  result += "Bucket [id=" + (std::string)id_ + " nodeIds=(";
  for (size_t i = 0; i < node_ids_.size(); ++i) {
    result += node_ids_[i];

    if (i < node_ids_.size() - 1) {
      result += ",";
    }
  }

  result += ")]";

  return result;
}

}  // namespace kademlia
}  // namespace cognitio
