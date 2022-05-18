// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "grpc_wrapper/client/client.hpp"

namespace cognitio {
namespace rpc {
namespace client {

grpc::ChannelArguments default_channel_arguments() {
    grpc::ChannelArguments arguments;
    arguments.SetMaxReceiveMessageSize(std::numeric_limits<int>::max());
    return arguments;
}

template <typename Service>
Client<Service>::~Client() {
  Kill();
}

template <typename Service>
void Client<Service>::ChangeServer(
    const std::string& address,
    ConnectionChangeCallback connection_change_callback) {
  // Disconnect from the previous server
  Kill();

  // Set the non-shared data
  server_address_ = address;
  queue_ = std::make_unique<grpc::CompletionQueue>();

  bool state_changed = false;
  ClientState cnc_client_state;

  // Update the shared data
  shared_data_.use_safely([&](SharedData& data) {
    // Create new channel to establish a connection
    data.channel = grpc::CreateCustomChannel(
        server_address_, grpc::InsecureChannelCredentials(),
        client::default_channel_arguments());
    data.stub = Service::NewStub(data.channel);

    // Get the current connection state and check if it has changed
    auto new_state = data.channel->GetState(true);

    cnc_client_state = to_cnc_client_state(new_state);
    state_changed =
        (cnc_client_state != to_cnc_client_state(data.connection_state));

    data.connection_state = new_state;

    // Ask the channel to notify us when state changes by updating 'queue_'
    auto deadline = std::chrono::system_clock::now() + std::chrono::seconds(15);
    data.channel->NotifyOnStateChange(data.connection_state, deadline,
                                      queue_.get(), connection_change_tag);
  });

  if (state_changed) {
    connection_change_callback(cnc_client_state);
  }

  // Wait for state changes
  run_thread_ = std::make_unique<std::thread>(
      &Client<Service>::run, this, std::move(connection_change_callback));
}

template <typename Service>
void Client<Service>::ChangeServer(
    std::shared_ptr<grpc::Channel> in_process_channel) {
  // Disconnect from the previous server
  Kill();

  server_address_ = "In-Process";

  shared_data_.use_safely(
      [in_process_channel = std::move(in_process_channel)](SharedData& data) {
        data.channel = in_process_channel;
        data.stub = Service::NewStub(data.channel);

        // No connectivity updates need to happen because the server is running
        // in the same process
        data.connection_state = GRPC_CHANNEL_READY;
      });
}

template <typename Service>
void Client<Service>::Kill() {
  shared_data_.use_safely([](SharedData& data) {
    // Delete the stub and channel first to trigger the shutdown events in the
    // channel. ('data.stub' has a shared pointer to channel so it needs to be
    // deleted too)
    data.stub = nullptr;
    data.channel = nullptr;
  });

  // Tell the queue to exit once all its current items have been popped
  if (queue_) {
    queue_->Shutdown();
  }

  // Wait for the thread to process the remaining events and exit
  if (run_thread_ && run_thread_->joinable()) {
    run_thread_->join();
  }
}

template <typename Service>
const std::string& Client<Service>::GetServerAddress() const {
  return server_address_;
}

template <typename Service>
detail::ClientState Client<Service>::GetState() {
  grpc_connectivity_state state_copy;
  shared_data_.use_safely(
      [&](const SharedData& data) { state_copy = data.connection_state; });
  return to_cnc_client_state(state_copy);
}

template <typename Service>
template <typename UsageFunc>
bool Client<Service>::UseStub(const UsageFunc& usage_func) {
  bool stub_valid = false;

  shared_data_.use_safely([&](const SharedData& data) {
    if (data.connection_state == GRPC_CHANNEL_READY) {
      stub_valid = true;
      usage_func(*data.stub);
    }
  });

  return stub_valid;
}

// This function is run from the 'run_thread_' thread
template <typename Service>
void Client<Service>::run(const std::function<void(const ClientState&)>&
                                  connection_change_callback) {
  ClientState cnc_client_state;

  void* current_tag;  // A label so se can identify the current update
  bool result_ok;     // Set to false if the queue receives updates due to
                      // cancellation (like hitting our deadline)

  while (queue_->Next(&current_tag, &result_ok)) {
    // The queue is currently only set up to receive connection changes
    assert(current_tag == connection_change_tag);

    bool state_changed = false;

    shared_data_.use_safely([&](SharedData& data) {
      if (data.channel) {  // not shutdown yet

        if (result_ok) {
          auto old_client_state = to_cnc_client_state(data.connection_state);
          data.connection_state = data.channel->GetState(true);
          cnc_client_state = to_cnc_client_state(data.connection_state);

          state_changed = (cnc_client_state != old_client_state);

          if (state_changed) {
            if (data.connection_state == GRPC_CHANNEL_READY) {
              for (auto& stream_pair : data.streams) {
                stream_pair.second->start_stream(*data.stub);
              }
            } else {
              for (auto& stream_pair : data.streams) {
                stream_pair.second->stop_stream();
              }
            }
          }
        }

        // Tell the channel to keep sending connection updates using this same
        // queue
        auto deadline = std::chrono::high_resolution_clock::now() +
                        std::chrono::seconds(60);
        data.channel->NotifyOnStateChange(data.connection_state, deadline,
                                          queue_.get(), connection_change_tag);

      } else if (to_cnc_client_state(data.connection_state) !=
                 detail::ClientState::not_connected) {
        // The channel has been shutdown but the state is not set so do not
        // disconnected yet. Set it appropriately.
        data.connection_state = GRPC_CHANNEL_SHUTDOWN;
        cnc_client_state = to_cnc_client_state(data.connection_state);
        state_changed = true;
      }
    });

    // Do the callback outside the locked code to prevent the user from
    // deadlocking the program.
    if (state_changed) {
      connection_change_callback(cnc_client_state);
    }
  }
}

}  // namespace client
}  // namespace rpc
}  // namespace cognitio
