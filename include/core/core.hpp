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
#include "grpc_wrapper/server/server.hpp"
#include "linked_data/merkle_dag.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace core {

using namespace config;

// TODO add block swap
class Core {
 public:
  typedef std::shared_ptr<repo::Repo<std::string>> RepoPtr;
  typedef std::shared_ptr<linked_data::MerkleDag> DagPtr;
  typedef std::unique_ptr<rpc::server::Server> ServerPtr;

  explicit Core(const std::string& repo_path) {
    repo_ = std::make_shared<repo::Repo<std::string>>(repo_path);
    dag_ = std::make_shared<linked_data::MerkleDag>(
        std::make_shared<exchange::BlockService>(repo_));

    server_ = std::make_unique<rpc::server::Server>();
    logger_ = common::logger::createLogger("core");
  }

  DagPtr GetDag() noexcept { return dag_; }
  RepoPtr GetRepo() noexcept { return repo_; }
  Status RunDaemon(std::vector<rpc::server::ServiceInfo> &vec) noexcept;
  void Shutdown() noexcept;

 private:
  void listen_shutdown();

  ServerPtr server_;
  RepoPtr repo_;
  DagPtr dag_;

  common::logger::Logger logger_;
};

}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_HPP_
