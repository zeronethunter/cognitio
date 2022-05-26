// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core.hpp"

#include <memory>

#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace core {

Status Core::RunDaemon(std::vector<rpc::server::ServiceInfo> &vec) noexcept {
  if (server_) {
    logger_->info("Starting daemon...");
    auto err = server_->Run(vec);
    if (!err.ok()) {
      logger_->error("Unable to start the server...");
      return Status(StatusCode::FAILED, "Server starting error");
    }
  } else {
    return Status(StatusCode::FAILED, "Server wasn't initialized");
  }

  // TODO: Start pinner and gc
  // ...

  logger_->info("Daemon is running");
  return Status::OK;
}

void Core::Shutdown() noexcept { return; }

}  // namespace core
}  // namespace cognitio
