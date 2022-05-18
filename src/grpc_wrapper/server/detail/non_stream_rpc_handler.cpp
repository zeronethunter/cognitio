// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "grpc_wrapper/server/detail/non_stream_rpc_handler.hpp"

namespace cognitio {
namespace rpc {
namespace server {
namespace detail {

template <typename Service, typename Request, typename Response,
          typename Callback>
NonStreamRpcHandler<Service, Request, Response, Callback>::NonStreamRpcHandler(
    Service& service, grpc::ServerCompletionQueue& server_queue,
    AsyncNoStreamFunc<Service, Request, Response> stream_func,
    Callback callback)
    : service_(service),
      server_queue_(server_queue),
      stream_func_(stream_func),
      callback_(std::move(callback)) {}

template <typename Service, typename Request, typename Response,
          typename Callback>
NonStreamRpcHandler<Service, Request, Response,
                    Callback>::~NonStreamRpcHandler() {
  queue_.Shutdown();

  void* ignored_tag;
  bool call_ok;
  while (queue_.Next(&ignored_tag, &call_ok)) {
    assert(not call_ok);
  }
}

template <typename Service, typename Request, typename Response,
          typename Callback>
void NonStreamRpcHandler<Service, Request, Response, Callback>::ActivateNext() {
  if (connection_) {
    Response response;
    grpc::Status status = callback_(connection_->request, &response);
    connection_->responder.Finish(response, status, &response);

    void* recv_tag;
    bool call_ok;

    if (queue_.Next(&recv_tag, &call_ok)) {
      assert(call_ok);
      assert(recv_tag == &response);
    } else {
      assert(false);
    }
  }

  // Add a new connection that is waiting to be activated
  connection_ = std::make_unique<NonStreamRpcConnection<Request, Response>>();
  (service_.*stream_func_)(&connection_->context, &connection_->request,
                           &connection_->responder, &queue_, &server_queue_,
                           this);
}

}  // namespace detail
}  // namespace server
}  // namespace rpc
}  // namespace cognitio
