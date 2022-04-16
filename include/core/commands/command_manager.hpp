// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_COMMAND_MANAGER_HPP_
#define CGNT_CORE_COMMANDS_COMMAND_MANAGER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "cli/commands/command.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace core {
namespace commands {

// FIXME: delete forward declaration when proto will be built
class CommandRequest;

struct UserCmd {
  std::unique_ptr<cognitio::cli::commands::Command> cmd;
  cli::commands::CmdEnv env;
};

class CommandManager {
 public:
  //! Initialize avail_arr vector with all available cli
  //! commands (add, get ...) (Check core/commands in order to
  //! know more about commands)
  CommandManager(std::unique_ptr<cognitio::cli::commands::Command>&& root)
      : root_(std::move(root)) {}

  //! \brief Creates UserCmd from request or command line arguments
  //! This method must be invoked after constructor
  Status InitRequest(const CommandRequest& req);
  Status InitArgv(const int& argc, const char* const argv[]);

  //! \brief Checks if the command was initialized successfully
  bool IsInitialized();

  //! \brief Getter for user_cmd
  UserCmd GetCmd() const;

  //! \brief Get user_cmd as Request
  //! It is used in remote Executor
  CommandRequest GetCmdAsRequest() const;

 private:
  //! \brief Used in order to find and check the commands
  //! \brief The root of the commands tree
  std::unique_ptr<cognitio::cli::commands::Command> root_;

  //! \brief Command with the options and arguments recieved from the user
  //! Environment must satisfy the Command's options and arguments
  UserCmd user_cmd_;
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_COMMAND_MANAGER_HPP_
