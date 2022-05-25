// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_GRPC_WRAPPER_SERVER_SERVER_HPP_
#define CGNT_GRPC_WRAPPER_SERVER_SERVER_HPP_

#include <grpcpp/grpcpp.h>

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "common/logger/logger.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace rpc {
namespace server {

using namespace common::logger;

struct ServiceInfo {
  std::string address;
  grpc::Service *service;
};

class Server {
 public:
  Server() = default;
  explicit Server(std::vector<ServiceInfo> &services) noexcept
      : services_(services) {}

  Server(Server &&other) noexcept;
  Server &operator=(Server &&other) noexcept;

  Status Run();
  void Shutdown();

 private:
  void init();
  void launch_and_wait();
  inline bool is_ready() const noexcept { return ready_; }

 private:
  std::vector<ServiceInfo> services_;

  std::thread thread_;
  grpc::ServerBuilder builder_;
  std::unique_ptr<grpc::Server> server_ = nullptr;

  std::mutex mutex_;
  bool ready_ = false;
  std::condition_variable cv_;

  std::atomic<bool> shutdown_ = false;
  Logger logger_ = createLogger("Server");
};

}  // namespace server
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_GRPC_WRAPPER_SERVER_SERVER_HPP_
