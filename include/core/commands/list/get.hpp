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

class GetMeta : public CmdMeta {
 public:
  GetMeta() : CmdMeta(std::string("get"), {}, false, true) {}
};

class GetCmd : public Command<Context> {
 public:
  GetCmd() : Command(GetMeta()) {};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}

  void Run(Context& ctx, [[maybe_unused]] CmdEnv& env,
           ResponseEmitter& re) override {
    common::Cid cid (env.option.c_str());
    auto api = ctx.GetAPI();
    api->Get(cid, re);
  }
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_LIST_GET_HPP_
