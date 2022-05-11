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

#include "common/concepts/container.hpp"
#include "common/status.hpp"

// FIXME: Delete forward declaration
class ResponseEmitter;

namespace cognitio {
namespace cli {
namespace commands {

// Composite patterns
template <class Context>
class Command {
  // TODO: sequence container concept to templates, not this hardcode
  typedef std::vector<std::unique_ptr<Command<Context>>> SubCmdsArr;

 public:
  class CmdMeta {
   public:
    typedef std::vector<std::string> ArgsArr;

    CmdMeta() = default;
    ~CmdMeta() = default;
    CmdMeta(std::string&& name, const ArgsArr& args = ArgsArr(),
            bool options_required = false, bool no_remote = false,
            bool no_local = false)
        : name_(name),
          possible_arguments_(args),
          options_required_(options_required),
          no_remote_(no_remote),
          no_local_(no_local) {}

    std::string GetName() const noexcept { return name_; }
    bool IsNoRemote() const noexcept { return no_remote_; }
    bool IsNoLocal() const noexcept { return no_local_; }

   private:
    std::string name_;
    ArgsArr possible_arguments_;
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
    Context ctx;
    CmdEnv env;
  };

  Command() = default;
  virtual ~Command() = default;
  CmdMeta& GetMeta() const noexcept { return meta_; }
  void AddSubCmd(std::unique_ptr<Command> cmd) { sub_commands_.push_back(cmd); }
  virtual Status Run(Context& ctx, const CmdEnv& env, ResponseEmitter& re) = 0;
  virtual void PrintHelp(std::ostream& out) = 0;

 private:
  CmdMeta meta_;
  SubCmdsArr sub_commands_;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_COMMAND_HPP_
