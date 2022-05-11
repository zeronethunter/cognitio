// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
#define CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP

#include "cli/commands/command.hpp"
#include "common/status_code.hpp"
#include "proto/cli/ProtoResponseEmitter.pb.h"

namespace cognitio {
namespace cli {
namespace commands {

class ResponseEmitter {
 public:
  ResponseEmitter() = default;
  ResponseEmitter(ResponseEmitter& re) = delete;
  ResponseEmitter& operator=(ResponseEmitter& re) = delete;
  ~ResponseEmitter() = default;

  void Append(/* ... */);
  void SetStatus(StatusCode code, std::string msg = "");

  virtual void Emit() = 0;

 private:
  ProtoResponse proto_;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_RESPONSE_EMITTER_HPP
