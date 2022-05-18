// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_COMMANDS_COMMAND_HPP_
#define CGNT_CLI_COMMANDS_COMMAND_HPP_

#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

#include "cli/commands/response_emitter.hpp"
#include "common/concepts/container.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace cli {
namespace commands {

struct CmdEnv {
  std::string option;
  std::map<std::string, std::string> arguments;
};

class CmdMeta {
 public:
  typedef std::map<std::string, std::string> ArgsArr;

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
  bool IsRepoRequired() const noexcept { return repo_required; }
  bool AreOptionsRequired() const noexcept { return options_required_; }
  const ArgsArr& GetDefaultArgs() const noexcept { return possible_arguments_; }

 private:
  std::string name_;
  ArgsArr possible_arguments_;
  bool options_required_ = false;
  bool repo_required = false;
  bool no_remote_ = false;
  bool no_local_ = false;
};

// Composite patterns
template <class Context>
class Command {
 public:
  typedef CmdMeta::ArgsArr ArgsArr;
  typedef std::shared_ptr<Command<Context>> CmdPtr;
  typedef std::vector<CmdPtr> SubCmdsArr;

  Command(CmdMeta meta = CmdMeta()) : meta_(meta) {}
  virtual ~Command() = default;

  const CmdMeta& GetMeta() const noexcept { return meta_; }
  std::string GetArgsPrefix() const noexcept { return std::string("--"); }
  CmdPtr GetSubCmd(const std::string& name) const;
  virtual void AddSubCmd(std::shared_ptr<Command> cmd) {
    sub_commands_.push_back(cmd);
  }

  virtual void Run(Context& ctx, CmdEnv& env, ResponseEmitter& re) = 0;
  virtual void PrintHelp(std::ostream& out) = 0;

 private:
  CmdMeta meta_;
  SubCmdsArr sub_commands_;
};

template <class Context>
struct CmdWrapper {
  std::unique_ptr<Command<Context>> cmd;
  CmdEnv env;
};

}  // namespace commands
}  // namespace cli
}  // namespace cognitio

#endif  // CGNT_CLI_COMMANDS_COMMAND_HPP_
