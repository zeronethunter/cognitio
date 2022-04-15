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

enum class GrpcClientState {
  not_connected,
  attempting_to_connect,
  connected,
};

inline std::string to_string(const GrpcClientState& state) {
  switch (state) {
    case GrpcClientState::not_connected:
      return "not_connected";
    case GrpcClientState::attempting_to_connect:
      return "attempting_to_connect";
    case GrpcClientState::connected:
      return "connected";
  }
  throw std::invalid_argument("Invalid GrpcClientState");
}

inline std::ostream& operator<<(std::ostream& os,
                                const GrpcClientState& state) {
  return os << to_string(state);
}

}  // namespace detail
}  // namespace client
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_CLIENT_DETAIL_CLIENT_STATE_HPP_
