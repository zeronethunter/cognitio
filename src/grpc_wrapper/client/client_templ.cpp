// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "client.cpp"
#include "core/core_api/api_service.hpp"
#include "grpc_wrapper/client/client.hpp"
#include "proto/api.grpc.pb.h"

namespace cognitio {
namespace rpc {
namespace client {

template class Client<CoreApiService>;

}
}  // namespace rpc
}  // namespace cognitio
