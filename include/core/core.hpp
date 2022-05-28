// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_HPP_
#define CGNT_CORE_CORE_HPP_

#include <memory>

#include "common/logger/logger.hpp"
#include "config/config.hpp"
#include "exchange/block_service/block_service.hpp"
#include "exchange/block_swap/block_swap.hpp"
#include "grpc_wrapper/server/server.hpp"
#include "kademlia/identifier.hpp"
#include "kademlia/kademlia.hpp"
#include "linked_data/merkle_dag.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace core {

using namespace config;

class Core {
 public:
  typedef std::shared_ptr<exchange::BlockService> BlockServicePtr;
  typedef std::shared_ptr<repo::Repo<std::string>> RepoPtr;
  typedef std::unique_ptr<rpc::server::Server> ServerPtr;
  typedef std::shared_ptr<linked_data::MerkleDag> DagPtr;
  typedef std::shared_ptr<exchange::BlockSwap> BsPtr;
  typedef std::shared_ptr<kademlia::Kademlia> DhtPtr;

  explicit Core(const std::string& repo_path);
  bool IsInit() const noexcept;
  void Init() noexcept;

  DhtPtr GetDht() noexcept { return dht_; }
  DagPtr GetDag() noexcept { return dag_; }
  RepoPtr GetRepo() noexcept { return repo_; }
  BsPtr GetBlockSwap() noexcept { return block_swap_; }

  void Shutdown() noexcept;
  Status RunDaemon(std::vector<rpc::server::ServiceInfo>& vec) noexcept;

 private:
  // For daemon launching
  void listen_shutdown();

  // All these modules are stored in the core
  // in order to be able to add sophisticated
  // commands in the future

  //! MerleDag builds graph from chunks and throws
  //! request below to block_service
  DagPtr dag_;

  //! Kademlia dht used in BlockSwap
  DhtPtr dht_;

  //! Storing and getting blocks to/from local repo
  RepoPtr repo_;

  //! Grpc server, initialized in the RunDaemon command
  ServerPtr server_;

  //! Getting nodes from the network
  BsPtr block_swap_;

  //! Used for repo initializtion
  std::string repo_path_;

  //! Aggregator for the Repo and BlockSwap
  BlockServicePtr block_service_;

  //! Just a logger
  common::logger::Logger logger_;
};

}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_HPP_
