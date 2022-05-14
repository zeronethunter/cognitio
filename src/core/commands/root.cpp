// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/commands/list/root.hpp"

#include "core/commands/list/init.hpp"

namespace cognitio {
namespace core {
namespace commands {

RootCmd MakeCommands() {
  RootCmd cmd;
  cmd.AddSubCmd(std::make_unique<InitCmd>(new InitCmd()));

  // Working with local repo
  cmd.AddSubCmd(std::make_unique<GetCmd>(new GetCmd()));
  cmd.AddSubCmd(std::make_unique<AddCmd>(new AddCmd()));
  cmd.AddSubCmd(std::make_unique<RmCmd>(new RmCmd()));
  // cmd.AddSubCmd(std::make_unique<DaemonCmd>(new DaemonCmd()));

  // Working with remote repo

  /*
  cmd.AddSubCmd(std::make_unique<PinCmd>(new PinCmd()));
  cmd.GetSubCmd("pin")->AddSubCmd(std::make_unique<PinGetCmd>(new PinGetCmd()));
  cmd.GetSubCmd("pin")->AddSubCmd(std::make_unique<PinAddCmd>(new PinAddCmd()));
  cmd.GetSubCmd("pin")->AddSubCmd(std::make_unique<PinRmCmd>(new PinRmCmd()));
  */

  return cmd;
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
