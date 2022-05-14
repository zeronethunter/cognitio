// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_HPP_
#define CGNT_CORE_CORE_HPP_

#include <memory>

#include "config/config.hpp"
#include "exchange/block_service/block_service.hpp"
#include "linked_data/merkle_dag.hpp"

namespace cognitio {
namespace core {

using namespace config;

class Core {
 public:
  typedef std::shared_ptr<Config> CfgPtr;
  typedef std::shared_ptr<exchange::BlockService> BsPtr;
  typedef std::shared_ptr<linked_data::MerkleDag> DagPtr;

  explicit Core(CfgPtr cfg) : config_(cfg) {
    block_service_ = std::make_shared<exchange::BlockService>();
    dag_ = std::make_shared<linked_data::MerkleDag>(block_service_);
  }

  DagPtr GetDag() noexcept { return dag_; }
  BsPtr GetBlockService() noexcept { return block_service_; }
  Status RunDaemon() noexcept;

 private:
  BsPtr block_service_;
  CfgPtr config_;
  DagPtr dag_;
};

}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_HPP_
