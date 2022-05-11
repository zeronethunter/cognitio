// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/context.hpp"

#include <cstdlib>

#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "common/utils/repo.hpp"
#include "core/core_api/core_api.hpp"

namespace cognitio {
namespace core {
namespace commands {
void Context::SetConfig(Config&& conf) noexcept {
  config_ = std::make_shared<Config>(std::move(conf));
}

void Context::SetAPI(CoreAPI&& api) noexcept {
  core_api_ = std::make_shared<CoreAPI>(std::move(api));
}

Status Context::Init(CmdMeta& meta, CmdEnv env) noexcept {
  std::string repo_path = getRepoPath(env);
}

std::string getRepoPath(CmdEnv& env) {
  std::string cgnt_home = std::getenv("CGNT_HOME");
  if (!cgnt_home.empty()) {
    return cgnt_home;
  }

  // auto if_found = std::find
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
