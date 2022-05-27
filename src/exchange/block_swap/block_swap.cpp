//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "exchange/block_swap/block_swap.hpp"

#include <cassert>
#include <cstdint>
#include <memory>

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
  auto conf = repo_->GetConfig();
  kademlia::ConnectionInfo info;
  info.InitWithString(id_, conf.Get("dht_address"));
  if (!dht_) {
    dht_ = std::make_shared<kademlia::Kademlia>(info);
  }

  dht_->Run();

  kademlia::ConnectionInfo boostrap;
  boostrap.InitWithString(kademlia::Identifier(rand()),
                          conf.Get("bootstrap_node_address"));
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
  auto conf = repo_->GetConfig();
  std::string my_address;
  if (conf.IsLocal()) {
    my_address = conf.Get("dht_address");
  } else {
    // TODO:
    my_address = conf.Get("dht_address");
  }

  dht_->Add(cid.ToString(), my_address);
}

}  // namespace exchange
}  // namespace cognitio
