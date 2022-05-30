// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/commands/list/debug/cid.hpp"

#include "common/status.hpp"
#include "common/status_code.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace core {
namespace commands {

void DebugCidCmd::Run([[maybe_unused]] Context& ctx, CmdEnv& env,
                      ResponseEmitter& re) {
  std::string path = env.option;
  std::vector<uint8_t> data;
  Status error(Status::OK);

  std::tie(data, error) = common::utils::get_bytes_from(path);
  if (error.ok()) {
    common::Cid cid(data);
    re.Append(cid.ToString());
  }

  error = Status(StatusCode::OK);
  re.SetStatus(StatusCode::OK);
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
