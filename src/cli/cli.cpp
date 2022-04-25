// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/cli.hpp"

#include "common/status.hpp"

namespace cognitio {
namespace cli {
Cli::Cli(RootCmd&& root, CliEnv&& env)
    : root_(std::move(root)), cli_env_(std::move(env)) {}

Status Cli::Run(std::vector<std::string>& args) {
  return Status::OK;
}

}  // namespace cli
}  // namespace cognitio
