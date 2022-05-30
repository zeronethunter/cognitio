// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "grpc_wrapper/server/server.hpp"

#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/impl/grpc_library.h>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace rpc {
namespace server {

Server::Server(Server &&other) noexcept
    :  // services_(std::move(other.services_)),
      thread_(std::move(other.thread_)),
      server_(std::move(other.server_)) {}

Server &Server::operator=(Server &&other) noexcept {
  if (this != &other) {
    // services_ = std::move(other.services_);
    thread_ = std::move(other.thread_);
    server_ = std::move(other.server_);
  }
  return *this;
}

Status Server::Run(std::vector<ServiceInfo> &services) {
  init(services);
  logger_->debug("Starting server");
  thread_ = std::thread(std::thread(&Server::launch_and_wait, this));

  std::unique_lock<std::mutex> lk(mutex_);
  cv_.wait(lk, [&] { return ready_; });

  if (server_ == nullptr) {
    return Status(StatusCode::FAILED, "failed to build and start the server");
  }

  return Status::OK;
}

void Server::Shutdown() {
  if (!shutdown_) {
    shutdown_ = true;
    if (server_) {
      server_->Shutdown(std::chrono::system_clock::now());
      server_->Wait();
    }

    if (thread_.joinable()) {
      thread_.join();
    }
  }
}

void Server::init(std::vector<ServiceInfo> &services) {
  grpc::EnableDefaultHealthCheckService(true);
  for (const auto &service : services) {
    try {
      auto str = service.address;
      builder_.AddListeningPort(str, grpc::InsecureServerCredentials());
    } catch (...) {
      logger_->error("Wrong format of the address: {}", service.address);
    }

    builder_.RegisterService(service.address, service.service);
  }

  builder_.SetMaxMessageSize(-1);
}

void Server::launch_and_wait() {
  {
    std::lock_guard<std::mutex> lk(mutex_);
    server_ = builder_.BuildAndStart();
    ready_ = true;
  }

  cv_.notify_one();
  if (server_ != nullptr) {
    server_->Wait();
  }
}

}  // namespace server
}  // namespace rpc
}  // namespace cognitio
