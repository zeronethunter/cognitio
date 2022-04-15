// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_FORWARD_DECLARATIONS_HPP_
#define CGNT_RPC_FORWARD_DECLARATIONS_HPP_

namespace cognitio {
namespace rpc {
namespace server {

using ClientID = void*;

class GrpcServer;

template <typename Service>
class GrpcAsyncServer;

}  // namespace server

namespace client {

template <typename Service>
class GrpcClient;

enum class GrpcClientState;

}  // namespace client
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_FORWARD_DECLARATIONS_HPP_
