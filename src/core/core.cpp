// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core.hpp"

#include <csignal>
#include <memory>

#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace core {

static std::atomic<bool> shutdown = false;
void signal_handler([[maybe_unused]] int signal) { shutdown = true; }

void Core::listen_shutdown() {
  while (true) {
    if (shutdown) {
      Shutdown();
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

Status Core::RunDaemon(std::vector<rpc::server::ServiceInfo> &vec) noexcept {
  if (server_) {
    logger_->info("Starting daemon...");
    auto err = server_->Run(vec);
    if (!err.ok()) {
      logger_->error("Unable to start the server...");
      return Status(StatusCode::FAILED, "Server starting error");
    }

    logger_->info("Server is running...");

    block_swap_->Run();
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
