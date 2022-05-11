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

#include "cli/commands/command.hpp"
#include "common/concepts/container.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"

namespace cognitio {
namespace cli {

using namespace common::logger;
using namespace commands;

template <class Context>
class Cli {
 public:
  typedef std::unique_ptr<commands::Command<Context>> CmdPtr;

  Cli() = default;
  explicit Cli(CmdPtr root);
  ~Cli() = default;

  bool IsInitialized();
  template <Container T>
  Status Run(T& args);

 private:
  Logger logger_ = createLogger("CLI");
  CmdPtr root_ = nullptr;
};
}  // namespace cli
}  // namespace cognitio

#endif
