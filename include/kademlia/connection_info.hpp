//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_CONNECTION_INFO_HPP_
#define CGNT_KADEMLIA_CONNECTION_INFO_HPP_

#include <string>

#include "kademlia/identifier.hpp"
#include "proto/kademlia.pb.h"

namespace cognitio {
namespace kademlia {

class ConnectionInfo {
 public:
  ConnectionInfo() = default;
  ~ConnectionInfo() = default;
  explicit ConnectionInfo(Identifier id, std::string ip, uint32_t port) {
    proto_.set_id(id);
    proto_.set_ip(ip);
    proto_.set_port(port);
  }

  Identifier id() const noexcept {
    return Identifier(std::atoi(proto_.id().c_str()));
  }

  ::ConnectionInfo GetProto() noexcept { return proto_; }
  std::string ip() const noexcept { return proto_.ip(); }
  uint32_t port() const noexcept { return proto_.port(); }
  std::string GetAddress() const noexcept {
    return std::string(proto_.ip() + ":" + std::to_string(proto_.port()));
  }

  friend bool operator==(const ConnectionInfo& A, const ConnectionInfo& B) {
    auto a_proto = A.proto_, b_proto = B.proto_;
    return a_proto.ip() == b_proto.ip() && a_proto.port() == b_proto.port() &&
           a_proto.id() == b_proto.id();
  }

  friend bool operator!=(const ConnectionInfo& A, const ConnectionInfo& B) {
    auto a_proto = A.proto_, b_proto = B.proto_;
    return a_proto.ip() != b_proto.ip() || a_proto.port() != b_proto.port() ||
           a_proto.id() != b_proto.id();
  }

 private:
  ::ConnectionInfo proto_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_CONNECTION_INFO_HPP_
