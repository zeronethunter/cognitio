// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_CLIENT_DETAIL_CLIENT_STATE_HPP_
#define CGNT_RPC_CLIENT_DETAIL_CLIENT_STATE_HPP_

#include <condition_variable>
#include <mutex>

namespace cognitio {
namespace rpc {
namespace client {
namespace detail {

enum class ClientState {
  not_connected,
  attempting_to_connect,
  connected,
};

inline std::string to_string(const ClientState& state) {
  switch (state) {
    case ClientState::not_connected:
      return "not_connected";
    case ClientState::attempting_to_connect:
      return "attempting_to_connect";
    case ClientState::connected:
      return "connected";
  }
  throw std::invalid_argument("Invalid ClientState");
}

inline std::ostream& operator<<(std::ostream& os,
                                const ClientState& state) {
  return os << to_string(state);
}

}  // namespace detail
}  // namespace client
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_CLIENT_DETAIL_CLIENT_STATE_HPP_
