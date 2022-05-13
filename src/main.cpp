// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <csignal>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>

#include "cli/cli.hpp"
#include "common/context.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "core/commands/list/root.hpp"

int main(int argc, char *argv[]) {
  using namespace cognitio;
  using namespace core::commands;

  std::srand(static_cast<uint>(std::time(0)));

  // Initial logger configuration
  auto logger = common::logger::createLogger("cognitio");
  logger->set_level(spdlog::level::debug);

  // Transforming args from c-style
  std::deque<std::string> args(argv + 1, argv + static_cast<size_t>(argc));
  // Builing the tree of commands, parsing & running of the argv
  cli::Cli<Context> cli(MakeCommands());
  Status status = cli.Run(args);
  return status.ok();
}
