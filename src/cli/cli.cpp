// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/cli.hpp"

#include <memory>
#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace cli {

template <class Context>
Cli<Context>::Cli(RootCmd&& root) : root_(std::make_unique<RootCmd>(root)) {}

template <class Context>
template <Container T>
Status Cli<Context>::Run(T& args) {
  logger_->debug("Command line interface module is starting...");
  if (!IsInitialized()) {
    logger_->error("CLI is not initialized with commands");
    return Status::FAILED;
  }

  CmdWrapper<Context> request;
  Status err = parse(args, request);
  if (!err.ok()) {
    logger_->error("Error while parsing: {}", err.error_message());
    return err;
  }

  err = handleHelp(request);
  if (err.ok()) {
    return Status::OK;
  }

  return Status::OK;
}

}  // namespace cli
}  // namespace cognitio
