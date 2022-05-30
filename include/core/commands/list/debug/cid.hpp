// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_COMMAND_DEBUG_CID_HPP_
#define CGNT_CORE_COMMANDS_COMMAND_DEBUG_CID_HPP_

#include <cassert>
#include <cstdint>
#include <fstream>

#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "core/context/context.hpp"
#include "common/utils/filesystem.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

class DebugCidMeta : public CmdMeta {
 public:
  DebugCidMeta() : CmdMeta(std::string("cid"), {}, false, true, false, false) {}
};

class DebugCidCmd : public Command<Context> {
 public:
  DebugCidCmd() : Command(DebugCidMeta()){};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}
  void Run([[maybe_unused]] Context& ctx, CmdEnv& env,
           ResponseEmitter& re) override {
    std::string path = env.option;
    std::vector<uint8_t> data;
    Status error (Status::OK);

    std::tie(data, error) = common::utils::get_bytes_from(path);
    if (error.ok()) {
      common::Cid cid(data);
      re.Append(cid.ToString());
    }

    error = Status(StatusCode::OK);
    re.SetStatus(StatusCode::OK);
  }
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_COMMAND_DEBUG_CID_HPP_
