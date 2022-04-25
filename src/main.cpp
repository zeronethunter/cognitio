// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "cli/cli.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "core/commands/command_root.hpp"

int main(int argc, char *argv[]) {
  using namespace cognitio;
  using namespace cognitio::cli;
  using namespace cognitio::common::logger;
  using namespace cognitio::core::commands;

  // Initializing needed stuff
  std::srand(static_cast<uint>(std::time(0)));
  LogSetSeverity(argc, argv);
  LogInit();

  // Transforming args from c-style to cpp
  std::vector<std::string> args(static_cast<size_t>(argc));
  std::transform(argv, argv + argc, args.begin(),
                 [&](char *arg) { return std::string(arg); });

  args[0] = "cognitio";
  Cli cli(MakeCommands(), {std::cin, std::cout, std::cerr});
  Status status = cli.Run(args);
  return status.ok();
}
