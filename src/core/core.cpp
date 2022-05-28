// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core.hpp"

#include <csignal>
#include <memory>
#include <ostream>

#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace core {

static std::atomic<bool> shutdown = false;
void signal_handler([[maybe_unused]] int signal) { shutdown = true; }

Core::Core(const std::string& repo_path)
    : dag_(nullptr),
      dht_(nullptr),
      repo_(nullptr),
      server_(nullptr),
      block_swap_(nullptr),
      repo_path_(repo_path),
      block_service_(nullptr) {
  logger_ = common::logger::createLogger("core");
}

void Core::Init() noexcept {
  repo_ = std::make_shared<repo::Repo<std::string>>(repo_path_);

  kademlia::ConnectionInfo info;
  auto dht_addr = repo_->GetConfig().Get("dht_address");
  if (!dht_addr.empty()) {
    info.InitWithString(kademlia::Identifier(rand()),
                        repo_->GetConfig().Get("dht_address"));
  } else {
    logger_->debug(
        "Dht wasn't properly initilized. "
        "Ignore this message if 'init' command lanched");
  }

  dht_ = std::make_shared<kademlia::Kademlia>(info);

  block_swap_ = std::make_shared<exchange::BlockSwap>(repo_, dht_, info.id());
  block_service_ = std::make_shared<exchange::BlockService>(repo_, block_swap_);

  dag_ = std::make_shared<linked_data::MerkleDag>(block_service_);
  server_ = std::make_unique<rpc::server::Server>();
}

bool Core::IsInit() const noexcept {
  return dag_ && dht_ && repo_ && server_ && block_swap_ &&
         !repo_path_.empty() && block_service_;
}

void Core::listen_shutdown() {
  while (true) {
    if (shutdown) {
      Shutdown();
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

Status Core::RunDaemon(std::vector<rpc::server::ServiceInfo>& vec) noexcept {
  if (server_) {
    logger_->info("Starting daemon...");
    auto err = server_->Run(vec);
    if (!err.ok()) {
      logger_->error("Unable to start the server...");
      return Status(StatusCode::FAILED, "Server starting error");
    }

    logger_->info("Server is running...");

    block_swap_->Run();
    block_service_->SetDaemonStatus(true);
    logger_->info("Block swap is running");

  } else {
    return Status(StatusCode::FAILED, "Server wasn't initialized");
  }

  logger_->info("Daemon is running");
  std::signal(SIGINT, signal_handler);
  auto shutdown_thread = std::thread(&Core::listen_shutdown, this);
  shutdown_thread.join();
  return Status::OK;
}

void Core::Shutdown() noexcept {
  logger_->info("Shutting down daemon...");
  if (server_) {
    server_->Shutdown();
  }

  if (block_swap_) {
    block_swap_->Shutdown();
  }
}

}  // namespace core
}  // namespace cognitio
