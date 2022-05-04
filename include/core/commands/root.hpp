// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_
#define CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_

#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "core/commands/context.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

class RootCmd : public Command<Context> {
 public:
  Status Run([[maybe_unused]] Context& ctx, [[maybe_unused]] const CmdEnv& env,
             [[maybe_unused]] ResponseEmitter& re) {
    return Status::OK;
  }
};

RootCmd MakeCommands();

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_COMMAND_ROOT_HPP_
