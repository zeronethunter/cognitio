// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_SERVER_ASYNC_CLIENT_HPP_
#define CGNT_RPC_SERVER_ASYNC_CLIENT_HPP_

#include <grpc++/security/server_credentials.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>

#include "rpc/server/detail/non_stream_rpc_handler.hpp"
#include "common/atomic_data.hpp"

namespace cognitio {
namespace rpc {
namespace server {

template <typename Request>
using DefaultStreamCallback = void (*)(const Request&);

template <typename Service>
class AsyncServer {
 public:
  explicit AsyncServer(std::shared_ptr<Service> service,
                       const std::string& address);
  ~AsyncServer();

  template <typename BaseService, typename Request, typename Response,
            typename Callback>
  void RegisterAsync(
      AsyncNoStreamFunc<BaseService, Request, Response> no_stream_func,
      Callback&& callback);

  // This is also called in the destructor
  void ShutdowAndWait();

  template <typename Duration>
  void ForceShutdowIn(Duration duration);

  grpc::Server& Server();

 private:
  std::shared_ptr<Service> service_;
  std::unique_ptr<grpc::ServerCompletionQueue> server_queue_;
  std::unique_ptr<grpc::Server> server_;

  using RpcMap =
      std::unordered_map<void*,
                         std::unique_ptr<detail::AsyncRpcHandlerInterface>>;

  cognitio::common::AtomicData<RpcMap> rpc_handlers_;
  std::thread run_thread_;
};

}  // namespace server
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_SERVER_ASYNC_CLIENT_HPP_
