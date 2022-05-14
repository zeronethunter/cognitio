// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_
#define CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_

#include "cli/commands/command.hpp"
#include "core/context/context.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

class RootMeta : public CmdMeta {
 public:
  RootMeta() : CmdMeta(std::string("cognitio")) {}
};

class RootCmd : public Command<Context> {
 public:
  RootCmd() : Command(RootMeta()){};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}
  void Run([[maybe_unused]] Context& ctx, [[maybe_unused]] const CmdEnv& env,
           [[maybe_unused]] ResponseEmitter& re) override {
    // It's not supposed to be ran
    re.SetStatus(StatusCode::FAILED, "I'm not supposed to be ran");
  }
};

RootCmd MakeCommands();

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_
