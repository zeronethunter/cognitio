// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_COMMAND_HPP_
#define CGNT_CLI_COMMANDS_COMMAND_HPP_

#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "common/status.hpp"

// FIXME: Delete forward declaration
class ResponseEmitter;

namespace cognitio {
namespace cli {
namespace commands {

// Composite patterns
template <class Context>
class Command {
 public:
  class CmdMeta {
   public:
    CmdMeta() = default;
    std::string GetName() const noexcept { return name_; }
    bool IsNoRemote() const noexcept { return no_remote_; }
    bool IsNoLocal() const noexcept { return no_local_; }

   private:
    std::string name_;
    std::vector<std::string> possible_options_;
    bool options_required_ = false;
    bool no_remote_ = false;
    bool no_local_ = false;
  };

  struct CmdEnv {
    std::vector<std::string> options;
    std::vector<std::string> arguments;
  };

  struct CmdWrapper {
    std::unique_ptr<Command> cmd;
    CmdEnv env;
  };

  Command() = default;
  virtual ~Command() = default;
  CmdMeta& GetMeta() const noexcept { return meta_; }
  void AddSubCmd(std::unique_ptr<Command> cmd) { sub_commands_.push_back(cmd); }
  virtual Status Run(Context& ctx, const CmdEnv& env, ResponseEmitter& re) = 0;

 private:
  CmdMeta meta_;
  std::vector<std::unique_ptr<Command<Context>>> sub_commands_;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_COMMAND_HPP_
