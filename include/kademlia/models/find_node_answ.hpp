//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_MODELS_FIND_NODE_ANSW_HPP_
#define CGNT_KADEMLIA_MODELS_FIND_NODE_ANSW_HPP_

#include <vector>

#include "kademlia/connection_info.hpp"
#include "kademlia/identifier.hpp"

namespace cognitio {
namespace kademlia {

class FindNodeAnswer {
 public:
  FindNodeAnswer();
  FindNodeAnswer(const Identifier& destinationId);
  ~FindNodeAnswer();

  std::vector<ConnectionInfo> getNodes() const;

  size_t size() const;
  void remove(const ConnectionInfo& element);
  void add(const ConnectionInfo& connection_info);
  void merge(const FindNodeAnswer& other);
  void prepare();

  bool found() const;

  bool contains(const ConnectionInfo& connection_info) const;

  friend bool operator==(const FindNodeAnswer& A, const FindNodeAnswer& B);
  friend bool operator!=(const FindNodeAnswer& A, const FindNodeAnswer& B);

  std::vector<ConnectionInfo>::iterator begin();
  std::vector<ConnectionInfo>::iterator end();
  std::vector<ConnectionInfo>::const_iterator cbegin() const;
  std::vector<ConnectionInfo>::const_iterator cend() const;

 private:
  Identifier dest_;
  std::vector<ConnectionInfo> closest_nodes_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_MODELS_FIND_NODE_ANSW_HPP_
