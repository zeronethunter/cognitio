// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/cli.hpp"

#include <algorithm>
#include <memory>

#include "cli/commands/command.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "core/commands/list/root.hpp"
#include "core/commands/local_emitter.hpp"

#define HELP_ARG_MSG "--help"
#define HELP_OPT_MSG "help"

namespace cognitio {
namespace cli {

template <class Context>
Cli<Context>::Cli(Command<Context>&& root, std::ostream& out)
    : out_(out),
      logger_(createLogger("CLI")),
      root_(std::make_shared<Command<Context>>(root)) {}

template <class Context>
template <Container T>
Status Cli<Context>::Run(T& args) const {
  logger_->debug("Command line interface module is starting...");

  CmdWrapper<Context> request;
  Status err = parse(args, request);

  {
    Status help_err = handleHelp(request);
    if (help_err.ok()) {
      logger_->debug("Help cmd invoked");
      return Status::OK;
    }
  }

  if (!err.ok()) {
    // Such a late checking of return value is done on purpose
    logger_->error("Error while parsing: '{}'", err.error_message());
    return err;
  }

  Context ctx;
  err = ctx.Init(request.cmd->GetMeta(), request.env);
  if (!err.ok() || !ctx.IsInitialized()) {
    logger_->error(err.error_message());
    return err;
  }

  LocalEmitter re(std::cout);
  request.cmd->Run(ctx, request.env, re);
  err = re.GetStatus();
  if (!err.ok()) {
    logger_->error(err.error_message());
    return err;
  }

  if (re.HaveData()) {
    re.Emit();
  }

  return Status::OK;
}

template <class Context>
template <Container T>
Status Cli<Context>::parse(T& args, CmdWrapper<Context>& cmdw) const noexcept {
  if (!args.size()) {
    root_->PrintHelp(std::cout);
    return Status(StatusCode::FAILED, "No command was specified");
  }

  auto err = parseCommand(args, cmdw.cmd);
  if (cmdw.cmd == root_) {
    return !err.ok() ? err
                     : Status(StatusCode::FAILED,
                              "No command with name {} found", args[0]);
  }

  err = parseArguments(args, cmdw.cmd, cmdw.env.arguments);
  if (!err.ok()) {
    return err;
  }

  cmdw.env.options(args.begin(), args.end());
  if (!cmdw.env.options.size() && cmdw.cmd->GetMeta().AreOptionsRequired()) {
    return Status(StatusCode::FAILED, "Options are requred");
  }

  return Status::OK;
}

template <class Context>
template <Container T>
Status Cli<Context>::parseCommand(T& args, CmdPtr cmd) const noexcept {
  auto current_cmd = root_;
  bool is_found = false;

  do {
    auto cmds_names = current_cmd->GetSubCmdsNames();
    auto found = std::find(cmds_names.begin(), cmds_names.end(), *args.begin());
    if (found == cmds_names.end()) {
      is_found = true;
      args.pop_front();
      current_cmd = current_cmd->GetSubCmd(*args.begin());
    } else {
      is_found = false;
      size_t next_idx = 1;  // checking the next arg
      std::string prefix = root_.GetArgsPrefix();
      if (args.size() &&
          args[next_idx].compare(0, prefix.size(), prefix) != 0 &&
          !current_cmd->GetMeta().AreOptionsRequired()) {
        return Status(StatusCode::FAILED, "Unknown command {}", args[next_idx]);
      }
    }
  } while (is_found);

  cmd = current_cmd;
  return Status::OK;
}

template <class Context>
template <Container T>
Status Cli<Context>::parseArguments(T& args, CmdPtr cmd,
                                    ArgsArr& arguments) const noexcept {
  // TODO
  arguments = cmd->GetMeta().GetDefaultArgs();
  for (const auto& x : args) {
    arguments.insert({x, "true"});
  }

  return Status::OK;
}

template <class Context>
Status Cli<Context>::handleHelp(CmdWrapper<Context>& cmdw) const noexcept {
  auto cmd_args = cmdw.env.arguments;
  std::pair<std::string, std::string> help_arg = {HELP_ARG_MSG, ""};
  auto find_arg = std::find(cmd_args.begin(), cmd_args.end(), help_arg);

  auto cmd_opts = cmdw.env.options;
  auto find_opt = std::find(cmd_opts.begin(), cmd_opts.end(), HELP_OPT_MSG);

  if (find_arg != cmd_args.end() || find_opt != cmd_opts.end()) {
    cmdw.cmd->PrintHelp();
    return Status::OK;
  }

  return Status::FAILED;
}

}  // namespace cli
}  // namespace cognitio
