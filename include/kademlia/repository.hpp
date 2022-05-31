//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_REPOSITORY_HPP_
#define CGNT_KADEMLIA_REPOSITORY_HPP_

#include <unordered_map>

#include "kademlia/identifier.hpp"
#include "kademlia/utils.hpp"

namespace cognitio {
namespace kademlia {

template <class T>
class KademliaRepository {
 public:
  KademliaRepository(const Identifier& ownerID) : ownerID_(ownerID) {}
  ~KademliaRepository() = default;

  bool contains(const std::string& key) const {
    auto it = keymap_.find(key);
    return it != keymap_.end();
  }

  bool store_optional(const std::string& key, const T& value) {
    if (contains(key)) {
      return false;
    }

    store(key, value);
    return true;
  }

  void store(const std::string& key, const T& value) { keymap_[key] = value; }

  const T& get_optional(const std::string& key, bool& exists) {
    if (!contains(key)) {
      exists = false;
      return T();
    }

    exists = true;
    return keymap_[key];
  }

  const T& get(const std::string& key) {
    if (!contains(key)) {
      // throw std::runtime_error("No key-value found.");
      return T();
    }

    return keymap_[key];
  }

  Identifier::numeric_type count() const {
    return (Identifier::numeric_type)keymap_.size();
  }

 private:
  Identifier ownerID_;
  std::unordered_map<std::string, T> keymap_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_REPOSITORY_HPP_
