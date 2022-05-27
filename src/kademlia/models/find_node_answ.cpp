//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/models/find_node_answ.hpp"

namespace cognitio {
namespace kademlia {

FindNodeAnswer::FindNodeAnswer() {}
FindNodeAnswer::FindNodeAnswer(const Identifier& destinationId)
    : dest_(destinationId) {}
FindNodeAnswer::~FindNodeAnswer() {}

std::vector<ConnectionInfo> FindNodeAnswer::getNodes() const {
  return closest_nodes_;
}

size_t FindNodeAnswer::size() const { return closest_nodes_.size(); }

void FindNodeAnswer::remove(const ConnectionInfo& element) {
  std::vector<ConnectionInfo>::iterator element_iterator =
      std::find_if(closest_nodes_.begin(), closest_nodes_.end(),
                   [element](const ConnectionInfo& current) -> bool {
                     return current.id() == element.id();
                   });

  if (element_iterator != closest_nodes_.end()) {
    closest_nodes_.erase(element_iterator);
  }
}

void FindNodeAnswer::add(const ConnectionInfo& connection_info) {
  closest_nodes_.push_back(connection_info);
}

void FindNodeAnswer::merge(const FindNodeAnswer& other) {
  for (auto iterator = other.cbegin(); iterator != other.cend(); iterator++) {
    if (!contains(*iterator)) {
      closest_nodes_.push_back(*iterator);
    }
  }
}

void FindNodeAnswer::prepare() {
  std::sort(closest_nodes_.begin(), closest_nodes_.end(),
            [this](const ConnectionInfo& A, const ConnectionInfo& B) -> bool {
              return A.id().distance(dest_) < B.id().distance(dest_);
            });

  if (closest_nodes_.size() > FIND_NODE_SIZE) {
    closest_nodes_.erase(closest_nodes_.begin() + FIND_NODE_SIZE,
                         closest_nodes_.begin() + closest_nodes_.size());
  }
}

bool FindNodeAnswer::found() const {
  if (size() < 1) {
    return false;
  }

  return closest_nodes_[0].id().distance(dest_) == 0;
}

bool FindNodeAnswer::contains(const ConnectionInfo& connection_info) const {
  auto iterator =
      std::find_if(closest_nodes_.begin(), closest_nodes_.end(),
                   [connection_info](const ConnectionInfo& element) -> bool {
                     return element == connection_info;
                   });

  return iterator != closest_nodes_.end();
}

std::vector<ConnectionInfo>::iterator FindNodeAnswer::begin() {
  return closest_nodes_.begin();
}

std::vector<ConnectionInfo>::iterator FindNodeAnswer::end() {
  return closest_nodes_.end();
}

std::vector<ConnectionInfo>::const_iterator FindNodeAnswer::cbegin() const {
  return closest_nodes_.cbegin();
}

std::vector<ConnectionInfo>::const_iterator FindNodeAnswer::cend() const {
  return closest_nodes_.cend();
}

bool operator==(const FindNodeAnswer& A, const FindNodeAnswer& B) {
  if (A.dest_ != B.dest_) {
    return false;
  }

  if (A.size() != B.size()) {
    return false;
  }

  for (int i = 0; i < A.size(); i++) {
    if (!B.contains(A.closest_nodes_[i])) {
      return false;
    }
  }

  return true;
}

bool operator!=(const FindNodeAnswer& A, const FindNodeAnswer& B) {
  return !(A == B);
}

}  // namespace kademlia
}  // namespace cognitio
