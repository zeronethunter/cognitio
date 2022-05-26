// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/commands/list/root.hpp"

#include "core/commands/list/add.hpp"
#include "core/commands/list/get.hpp"
#include "core/commands/list/init.hpp"
#include "core/commands/list/remove.hpp"
#include "core/commands/list/daemon.hpp"

namespace cognitio {
namespace core {
namespace commands {

RootCmd MakeCommands() {
  RootCmd cmd;
  cmd.AddSubCmd(std::make_shared<InitCmd>());

  // Working with local repo
  cmd.AddSubCmd(std::make_shared<GetCmd>());
  cmd.AddSubCmd(std::make_shared<AddCmd>());
  cmd.AddSubCmd(std::make_shared<RmCmd>());
  cmd.AddSubCmd(std::make_shared<DaemonCmd>());

  return cmd;
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
