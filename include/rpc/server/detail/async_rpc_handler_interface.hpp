// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <grpc++/completion_queue.h>
#include <grpc++/server_context.h>

namespace cognitio {
namespace rpc {
namespace server {

//! \brief The function signature for a service's non-streaming calls
template <typename Service, typename Request, typename Response>
using AsyncNoStreamFunc = void (Service::*)(
    grpc::ServerContext*, Request*, grpc::ServerAsyncResponseWriter<Response>*,
    grpc::CompletionQueue*, grpc::ServerCompletionQueue*, void*);

//! \brief The function signature for a service's server-side-streaming calls
template <typename Service, typename Request, typename Response>
using AsyncServerStreamFunc = void (Service::*)(
    grpc::ServerContext* context, Request*, grpc::ServerAsyncWriter<Response>*,
    grpc::CompletionQueue*, grpc::ServerCompletionQueue*, void*);

namespace detail {

class AsyncRpcHandlerInterface {
 public:
  virtual ~AsyncRpcHandlerInterface() = default;

  //!  \brief Tells the handler to call the appropriate service
  //!      function (AysncNoStreamFunc or AysncServerStreamFunc)
  //!
  //!  This will update the server's queue so client requests can be handled
  //!  when they are received.
  virtual void ActivateNext() = 0;
};

}  // namespace detail
}  // namespace server
}  // namespace rpc

}  // namespace cognitio
