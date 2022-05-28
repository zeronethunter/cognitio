//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "exchange/block_swap/block_swap.hpp"

#include <cassert>
#include <cstdint>
#include <memory>

#include "common/status.hpp"
#include "exchange/block_swap/client.hpp"
#include "kademlia/connection_info.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/kademlia.hpp"
#include "linked_data/proto_block.hpp"
#include "proto/kademlia.pb.h"
#include "repo/repo.hpp"

namespace cognitio {
namespace exchange {

void BlockSwap::Run() noexcept {

  dht_->Run();
  kademlia::ConnectionInfo boostrap;
  boostrap.InitWithString(kademlia::Identifier(rand()),
                          repo_->GetConfig().Get("bootstrap_node_address"));
  dht_->Bootstrap(boostrap);
}

linked_data::ProtoBlock BlockSwap::Get(const common::Cid& cid) noexcept {
  auto address = dht_->Get(cid.ToString());
  if (address.empty()) {
    return linked_data::ProtoBlock();
  }

  BlockSwapClient client(address);
  auto node = client.GetNode(cid.ToString());
  return node;
}

Status BlockSwap::Add(const common::Cid& cid) noexcept {
  std::string my_address;
  if (repo_->GetConfig().IsLocal()) {
    my_address = repo_->GetConfig().Get("dht_address");
  } else {
    // TODO:
    my_address = repo_->GetConfig().Get("dht_address");
  }

  return dht_->Add(cid.ToString(), my_address);
}

}  // namespace exchange
}  // namespace cognitio
