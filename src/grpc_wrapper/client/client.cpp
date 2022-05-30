// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "grpc_wrapper/client/client.hpp"

#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

namespace cognitio {
namespace rpc {
namespace client {

template <class Service>
Client<Service>::Client(const std::string& addr) : host_addr_(addr) {
  ChangeServer(host_addr_);
}

template <class Service>
void Client<Service>::ChangeServer(const std::string& address) {
  grpc::ChannelArguments ch_args;
  ch_args.SetMaxReceiveMessageSize(-1);

  stub_ = Service::NewStub(grpc::CreateCustomChannel(
      address, grpc::InsecureChannelCredentials(), ch_args));
}

}  // namespace client
}  // namespace rpc
}  // namespace cognitio
