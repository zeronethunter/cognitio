// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
#define CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP

#include <iostream>

#include "common/status.hpp"
#include "common/status_code.hpp"
#include "proto/api.pb.h"

namespace cognitio {
namespace cli {
namespace commands {

class ResponseEmitter {
 public:
  ResponseEmitter() = default;
  ResponseEmitter(ProtoResponse&& proto) : proto_(proto) {}
  ResponseEmitter(ResponseEmitter& re) = delete;
  ResponseEmitter& operator=(ResponseEmitter& re) = delete;
  ~ResponseEmitter() = default;

  void Append(const std::string& data);
  bool HaveData() const noexcept { return !proto_.data().empty(); }
  void SetStatus(Status& err);
  void SetStatus(StatusCode code, std::string msg = "");
  void ReplaceProto(const ProtoResponse& proto) { proto_ = proto; }
  ProtoResponse& GetProto() noexcept { return proto_; }
  Status GetStatus() const noexcept {
    auto code = static_cast<StatusCode>(proto_.status().statuscode());
    auto msg = proto_.status().statusmsg();
    return Status(code, msg);
  }

  void Emit(std::ostream& out) { out << proto_.data() << std::endl; }

 private:
  ProtoResponse proto_;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
