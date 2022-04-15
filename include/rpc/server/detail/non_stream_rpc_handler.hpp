// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_SERVER_DETAIL_NON_STREAM_RPC_HANDLER_HPP_
#define CGNT_RPC_SERVER_DETAIL_NON_STREAM_RPC_HANDLER_HPP_

#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "common/atomic_data.hpp"
#include "rpc/server/detail/async_rpc_handler_interface.hpp"
#include "rpc/server/detail/tag.hpp"

namespace cognitio {
namespace rpc {
namespace server {
namespace detail {

//! \brief Handles an individual non-streaming gRPC call when a client requests
//! this rpc
//!
//! \tparam Request is the Protobuf request type
//! \tparam Response is the Protobuf response type
template <typename Request, typename Response>
struct NonStreamRpcConnection {
  grpc::ServerContext context;
  Request request;
  grpc::ServerAsyncResponseWriter<Response> responder;

  NonStreamRpcConnection() : responder(&context) {
  }
};

//! \brief Handles non-streaming gRPC responses for a single rpc call
//
//! \tparam Service is the gRPC service
//! \tparam Request is the Protobuf request type
//! \tparam Response is the Protobuf response type
//! \tparam Callback is the implementation of this rpc call (signature:
//!  <grpc::Status(const Request&, Response*)>)
template <typename Service, typename Request, typename Response,
          typename Callback>
class NonStreamRpcHandler : public AsyncRpcHandlerInterface {
 public:
  explicit NonStreamRpcHandler(
      Service& service, grpc::ServerCompletionQueue& server_queue,
      AsyncNoStreamFunc<Service, Request, Response> stream_func,
      Callback callback);

  ~NonStreamRpcHandler() override;

  //! \see AsyncRpcHandlerInterface::ActivateNext()
  void ActivateNext() override;

 private:
  Service&
      service_;  //!< The gRPC service with the RPC call this class is handling
  grpc::ServerCompletionQueue&
      server_queue_;  //!< The queue that handles server updates
  AsyncNoStreamFunc<Service, Request, Response>
      stream_func_;    //!< The service function used to update the queue
  Callback callback_;  //!< The server specific implementation of this RPC call
  grpc::CompletionQueue queue_;  //!< Internal queue used to handle responses
                                 //!< (required for async api)

  //! All the data needed to handle the RPC call when a client make a request
  std::unique_ptr<NonStreamRpcConnection<Request, Response>> connection_;
};


}  // namespace detail
}  // namespace server
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_SERVER_DETAIL_NON_STREAM_RPC_HANDLER_HPP_
