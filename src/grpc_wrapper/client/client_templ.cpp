// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "client.cpp"
#include "grpc_wrapper/client/client.hpp"
#include "proto/ProtoData.grpc.pb.h"
#include "proto/api.grpc.pb.h"
#include "proto/kademlia.grpc.pb.h"

namespace cognitio {
namespace rpc {
namespace client {

template class Client<CoreApiService>;
template class Client<KademliaService>;
template class Client<BlockSwapService>;

}
}  // namespace rpc
}  // namespace cognitio
