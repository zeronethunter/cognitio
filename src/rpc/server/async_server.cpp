#include <rpc/server/async_server.hpp>

namespace cognitio {
namespace rpc {
namespace server {

template <typename Service>
AsyncServer<Service>::AsyncServer(std::shared_ptr<Service> service,
                                  const std::string& address)
    : service_(std::move(service)) {
  grpc::ServerBuilder builder;
  builder.RegisterService(service_.get());
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.SetMaxMessageSize(std::numeric_limits<int>::max());
  server_queue_ = builder.AddCompletionQueue();
  server_ = builder.BuildAndStart();

  run_thread_ = std::thread([&] {
    void* tag;
    bool call_ok;

    while (server_queue_->Next(&tag, &call_ok)) {
      if (call_ok) {
        rpc_handlers_.UseSafely([&](RpcMap& rpc_handlers) {
          rpc_handlers.at(tag)->activate_next();
        });
      } else {
        rpc_handlers_.UseSafely(
            [&](RpcMap& rpc_handlers) { rpc_handlers.erase(tag); });
      }
    }
  });
}

template <typename Service>
AsyncServer<Service>::~AsyncServer() {
  ShutdowAndWait();
  server_queue_->Shutdown();
  run_thread_.join();
}

template <typename Service>
template <typename BaseService, typename Request, typename Response,
          typename Callback>
void AsyncServer<Service>::RegisterAsync(
    AsyncNoStreamFunc<BaseService, Request, Response> no_stream_func,
    Callback&& callback) {
  static_assert(std::is_base_of<BaseService, Service>::value,
                "BaseService must be a base class of Service");
  auto handler = std::make_unique<
      detail::NonStreamRpcHandler<BaseService, Request, Response, Callback>>(
      *service_, *server_queue_, no_stream_func,
      std::forward<Callback>(callback));

  auto* tag = handler.get();
  rpc_handlers_.UseSafely([&](RpcMap& rpc_handlers) {
    rpc_handlers.emplace(tag, std::move(handler));
  });
  tag->activate_next();
}

template <typename Service>
void AsyncServer<Service>::ShutdowAndWait() {
  server_->Shutdown();
}

template <typename Service>
template <typename Duration>
void AsyncServer<Service>::ForceShutdowIn(Duration duration) {
  server_->Shutdown(std::chrono::system_clock::now() + duration);
}

template <typename Service>
grpc::Server& AsyncServer<Service>::Server() {
  return *server_;
}

}  // namespace server
}  // namespace rpc
}  // namespace cognitio
