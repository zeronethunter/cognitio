// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_COMMAND_HPP_
#define CGNT_CLI_COMMANDS_COMMAND_HPP_

#include <memory>
#include <string>
#include <vector>

#include "common/status.hpp"

// FIXME: Delete forward declaration
class ResponseEmitter;

namespace cognitio {
namespace cli {
namespace commands {

struct CmdEnv {
  std::vector<std::string> options;
  std::vector<std::string> arguments;
};

template <class Context>
class Command {
 public:
  Command() = default;
  virtual ~Command() = default;

  std::string GetName() const noexcept { return cmd_name_; }
  bool isRunnable() const noexcept { return is_runnable_; }
  bool isNoRemote() const noexcept { return no_remote_; }
  bool isNoLocal() const noexcept { return no_local_; }

  virtual Status Run(Context& ctx, const CmdEnv& env, ResponseEmitter& re) = 0;
  std::vector<std::unique_ptr<Command<Context>>> GetSubCommands() noexcept {
    return sub_commands_;
  }

 private:
  std::string cmd_name_;
  std::vector<std::string> options_;
  std::vector<std::unique_ptr<Command<Context>>> sub_commands_;

  bool is_runnable_ = false;
  bool no_remote_ = false;
  bool no_local_ = false;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_COMMAND_HPP_
