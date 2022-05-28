//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_EXCHANGE_BLOCK_SWAP_BLOCK_SWAP_HPP_
#define CGNT_EXCHANGE_BLOCK_SWAP_BLOCK_SWAP_HPP_

#include <cstdint>

#include "config/config.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/kademlia.hpp"
#include "linked_data/proto_block.hpp"
#include "multiformats/cid.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace exchange {

class BlockSwap {
 public:
  typedef std::shared_ptr<repo::Repo<std::string>> RepoPtr;
  typedef std::shared_ptr<kademlia::Kademlia> KademliaPtr;

  explicit BlockSwap(RepoPtr ptr) : repo_(ptr), dht_(nullptr), id_(rand()) {}
  explicit BlockSwap(RepoPtr ptr, KademliaPtr kad, kademlia::Identifier id)
      : repo_(ptr), dht_(kad), id_(id) {}

  ~BlockSwap() { Shutdown(); }

  void Run() noexcept;
  void Shutdown() noexcept {
    if (IsAlive()) {
      dht_->Shutdown();
    }
  }

  bool IsAlive() const noexcept { return dht_ && dht_->IsAlive(); }
  KademliaPtr GetDht() noexcept { return dht_; }

  linked_data::ProtoBlock Get(const common::Cid& cid) noexcept;
  Status Add(const common::Cid& cid) noexcept;

 private:
  RepoPtr repo_;
  KademliaPtr dht_;
  kademlia::Identifier id_;
};

}  // namespace exchange
}  // namespace cognitio

#endif  // CGNT_EXCHANGE_BLOCK_SWAP_BLOCK_SWAP_HPP_
