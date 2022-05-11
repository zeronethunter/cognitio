// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CLI_CLI_HPP_
#define CGNT_CLI_CLI_HPP_

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "cli/commands/command.hpp"
#include "common/concepts/container.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "core/commands/root.hpp"

namespace cognitio {
namespace cli {

using namespace common::logger;
using namespace core::commands;

template <class Context>
class Cli {
  typedef std::unique_ptr<core::commands::RootCmd> CmdPtr;

 public:
  Cli() = default;
  explicit Cli(RootCmd&& root);
  ~Cli() = default;

  bool IsInitialized();
  template <Container T>
  Status Run(T& args);

 private:
  template <Container T>
  Status parse(T& args, CmdWrapper<Context>& cmd);
  Status handleHelp(CmdWrapper<Context>& cmd);
<<<<<<< HEAD
=======

>>>>>>> 20c3cb5 (Template for Makefile's protobuf compiling)
  Logger logger_ = createLogger("CLI");
  CmdPtr root_ = nullptr;
};
}  // namespace cli
}  // namespace cognitio

#endif
