// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_CLI_HPP_
#define CGNT_CLI_CLI_HPP_

#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "common/status.hpp"
#include "core/commands/command_root.hpp"

namespace cognitio {
namespace cli {

using namespace core::commands;

class Cli {
 public:
  struct CliEnv {
    std::istream& in;
    std::ostream& out;
    std::ostream& err;
  };

  explicit Cli(RootCmd&& root, CliEnv&& env);
  Status Run(std::vector<std::string>& args);

 private:
  RootCmd root_;
  CliEnv cli_env_;
};
}  // namespace cli
}  // namespace cognitio

#endif
