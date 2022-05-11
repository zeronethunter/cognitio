// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/cli.hpp"

#include "cli/commands/command.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace cli {

template <class Context>
Cli<Context>::Cli(Cli<Context>::CmdPtr root) : root_(root) {}

template <class Context>
template <Container T>
Status Cli<Context>::Run(T& args) {
  logger_->debug("Command line interface module is starting...");

  typename Command<Context>::CmdWrapper request;
  Status err = parse(args, request);
  if (!err.ok()) {
    logger_->error("Error while parsing: {}", err.error_message());
    return err;
  }

  return Status::OK;
}

}  // namespace cli
}  // namespace cognitio
