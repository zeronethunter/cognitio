// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_CLIENT_CLIENT_HPP_
#define CGNT_RPC_CLIENT_CLIENT_HPP_

#include <grpc++/channel.h>
#include <grpc++/completion_queue.h>
#include <grpc++/create_channel.h>
#include <grpc++/server.h>

#include <thread>

#include "common/atomic_data.hpp"
#include "rpc/client/detail/client_state.hpp"
#include "rpc/forward_declarations.hpp"

namespace cognitio {
namespace rpc {
namespace client {

inline detail::ClientState to_cnc_client_state(grpc_connectivity_state state) {
  switch (state) {
    case GRPC_CHANNEL_SHUTDOWN:
    case GRPC_CHANNEL_IDLE:
      return detail::ClientState::not_connected;

    case GRPC_CHANNEL_CONNECTING:
    case GRPC_CHANNEL_TRANSIENT_FAILURE:
      return detail::ClientState::attempting_to_connect;

    case GRPC_CHANNEL_READY:
      return detail::ClientState::connected;
  }

  throw std::invalid_argument("Invalid grpc_connectivity_state");
}

template <typename Service>
class Client {
  using ConnectionChangeCallback = std::function<void(ClientState)>;

 public:
  explicit Client() = default;
  ~Client();

  //! \brief Attempt to connect to a host address in the form "server:port".
  //!
  //! The client will attempt to connect and as the connection state changes the
  //! 'connection_change_callback' will be invoked to inform the user of the
  //! changes.
  //!
  //! NOTE: The callback will be invoked from a separate thread.
  void ChangeServer(const std::string& address,
                    ConnectionChangeCallback connection_change_callback);

  //! \brief Create a direct channel to a server running in the same process.
  //!
  //! No connection callback is needed because the client will always be in a
  //! connected state
  //!
  void ChangeServer(std::shared_ptr<grpc::Channel> in_process_channel);

  //! \brief Stop all connection attempts or disconnect (if already connected)
  void Kill();

  const std::string& GetServerAddress() const;
  detail::ClientState GetState();

  //! \brief Safely use the service stub to make RPC calls.
  //!
  //! If the client is not connected this function will return false and
  //! 'usage_func' will not be invoked
  template <typename UsageFunc>
  bool UseStub(const UsageFunc& usage_func);

 private:
  //! \brief All the data shared between threads
  struct SharedData {
    grpc_connectivity_state connection_state = GRPC_CHANNEL_IDLE;
    std::shared_ptr<grpc::Channel> channel = nullptr;
    std::unique_ptr<typename Service::Stub> stub = nullptr;
  };

  //! Using atomic access to manipulate the shared data
  common::AtomicData<SharedData> shared_data_;

  std::string server_address_;

  //! Used as a label in 'queue_' so we know when the state changed.
  void* connection_change_tag = reinterpret_cast<void*>(0x1);

  // These are pointers so they can be reused after calling 'Shutdown' and
  // 'join' respectively.
  std::unique_ptr<grpc::CompletionQueue> queue_;
  std::unique_ptr<std::thread> run_thread_;

  void run(const std::function<void(const ClientState&)>&
               connection_change_callback);
};

}  // namespace client
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_CLIENT_CLIENT_HPP_
