// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_LIST_INIT_HPP_
#define CGNT_CORE_COMMANDS_LIST_INIT_HPP_

#include <cassert>
#include "cli/commands/command.hpp"
#include "core/context/context.hpp"
#include "core/core_api/local_api.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

class InitMeta : public CmdMeta {
 public:
  InitMeta() : CmdMeta(std::string("init"), {}, false, true) {}
};

class InitCmd : public Command<Context> {
 public:
  InitCmd() : Command(InitMeta()){};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}

  void Run(Context& ctx, [[maybe_unused]] CmdEnv& env,
           ResponseEmitter& re) override {
    assert(!ctx.GetCore()->IsInit());
    repo::Repo<std::string> repo(ctx.GetRepoPath());
    auto err = repo.Init();
    re.SetStatus(err);
  }
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif
