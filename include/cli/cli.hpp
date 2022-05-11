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

#define CmdPtr typename Command<Context>::CmdPtr
#define ArgsArr typename Command<Context>::ArgsArr
#define OptsArr typename Command<Context>::OptsArr

using namespace common::logger;
using namespace core::commands;

template <class Context>
class Cli {
 public:
  explicit Cli(RootCmd&& root, std::ostream& out = std::cout);
  ~Cli() = default;

  template <Container T>
  Status Run(T& args) const;

 private:
  template <Container T>
  Status parse(T& args, CmdWrapper<Context>& cmd);
  Status handleHelp(CmdWrapper<Context>& cmd);
  Status parse(T& args, CmdWrapper<Context>& cmdw) const noexcept;
  template <Container T>
  Status parseCommand(T& args, CmdPtr cmd) const noexcept;
  template <Container T>
  Status parseArguments(T& args, CmdPtr cmd, ArgsArr& arguments) const noexcept;
  Status handleHelp(CmdWrapper<Context>& cmdw) const noexcept;

  std::ostream& out_;
  Logger logger_;
  CmdPtr root_;
};
}  // namespace cli
}  // namespace cognitio

#endif
