// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_LIST_GET_HPP_
#define CGNT_CORE_COMMANDS_LIST_GET_HPP_

#include "cli/commands/command.hpp"
#include "core/context/context.hpp"
#include "common/multiformats/cid.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

class RmMeta : public CmdMeta {
 public:
  RmMeta() : CmdMeta(std::string("rm"), {}, false, true) {}
};

class RmCmd : public Command<Context> {
 public:
  RmCmd() : Command(RmMeta()){};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}

  void Run(Context& ctx, [[maybe_unused]] const CmdEnv& env,
           ResponseEmitter& re) override {
    common::Cid cid(env.option);
    auto api = ctx.GetAPI();
    api->Remove(cid, re);
  }
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_LIST_GET_HPP_
