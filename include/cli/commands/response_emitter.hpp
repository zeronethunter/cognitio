// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
#define CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP

#include "common/status.hpp"
#include "proto/ProtoResponseEmitter.pb.h"

namespace cognitio {
namespace cli {
namespace commands {

class ResponseEmitter {
 public:
  ResponseEmitter() = default;
  ResponseEmitter(ResponseEmitter& re) = delete;
  ResponseEmitter& operator=(ResponseEmitter& re) = delete;
  ~ResponseEmitter() = default;

  virtual void Emit() = 0;
  void Append(const std::string& data);
  bool HaveData() const noexcept { return proto_.data().empty(); }
  void SetStatus(StatusCode code, std::string msg = "");
  Status GetStatus() const noexcept {
    auto code = static_cast<StatusCode>(proto_.status().statuscode());
    auto msg = proto_.status().statusmsg();
    return Status(code, msg);
  }

 protected:
  ProtoResponse& GetProto() noexcept { return proto_; }

 private:
  ProtoResponse proto_;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
