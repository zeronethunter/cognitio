// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <csignal>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "cli/cli.hpp"
#include "common/context.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "core/commands/root.hpp"

int main(int argc, char *argv[]) {
  using namespace cognitio;
  using namespace core::commands;

  std::srand(static_cast<uint>(std::time(0)));

  // Initial logger configuration
  auto loggerProcessingEngine = common::logger::createLogger("cognitio");
  loggerProcessingEngine->set_level(spdlog::level::debug);

  // Transforming args from c-style to cpp
  std::vector<std::string> args(argv, argv + static_cast<size_t>(argc));
  if (argc <= 2) {
    args.push_back("--help");
  }

  // Output depends on executable name passed is args
  // So we need to make sure it's stable
  args[0] = "cognitio";

  // Builing the tree of commands, parsing & running of the argv
  cli::Cli<Context> cli(std::make_unique<RootCmd>(MakeCommands()),
                        {std::cin, std::cout, std::cerr});

  Status status = cli.Run(args);
  return status.ok();
}
