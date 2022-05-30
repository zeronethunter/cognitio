// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/commands/list/root.hpp"

#include <memory>

#include "core/commands/list/add.hpp"
#include "core/commands/list/daemon.hpp"
#include "core/commands/list/get.hpp"
#include "core/commands/list/remove.hpp"
#include "core/commands/list/repo/init.hpp"
#include "core/commands/list/repo/repo.hpp"
#include "core/commands/list/repo/reset.hpp"

#ifdef DEBUG_MODE
#include "core/commands/list/debug/debug.hpp"
#include "core/commands/list/debug/cid.hpp"
#endif

namespace cognitio {
namespace core {
namespace commands {

RootCmd MakeCommands() {
  RootCmd cmd;
  // Default commands
  cmd.AddSubCmd(std::make_shared<DaemonCmd>());
  cmd.AddSubCmd(std::make_shared<GetCmd>());
  cmd.AddSubCmd(std::make_shared<AddCmd>());
  cmd.AddSubCmd(std::make_shared<RmCmd>());

  // Repo commands
  cmd.AddSubCmd(std::make_shared<RepoCmd>());
  cmd.GetSubCmd("repo")->AddSubCmd(std::make_shared<InitCmd>());
  cmd.GetSubCmd("repo")->AddSubCmd(std::make_shared<ResetCmd>());

  // Not for the release version
  #ifdef DEBUG_MODE
  cmd.AddSubCmd(std::make_shared<DebugCmd>());
  cmd.GetSubCmd("debug")->AddSubCmd(std::make_shared<DebugCidCmd>());
  #endif

  return cmd;
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
