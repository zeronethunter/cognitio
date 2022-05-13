// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/context.hpp"

#include <algorithm>
#include <cstdlib>
#include <utility>

#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "core/core_api/core_api.hpp"

#define CONFIG_ARG "--config"

namespace cognitio {
namespace core {
namespace commands {

void Context::SetConfig(Config&& conf) noexcept {
  config_ = std::make_shared<Config>(std::move(conf));
}

void Context::SetAPI(CoreAPI&& api) noexcept {
  core_api_ = std::make_shared<CoreAPI>(std::move(api));
}

Status Context::Init(CmdMeta& meta, CmdEnv& env) noexcept {
  repo_path_ = getRepoPath(env);
  logger_->debug("Found repo path: {}", repo_path_);

  config_ = std::make_shared<Config>(new Config(repo_path_));
  auto err = config_.TryInit();
  if (!err.ok()) {
    logger_->warn("Unable to find config at {}", repo_path_);
    err = config_.Dump();
    if (!err.ok()) {
      return Status(StatusCode::FAILED,
                    "Unable to dump default config. Exiting...");
    } else {
      logger_->debug("Successfully dumped default config");
    }
  }

  err = resolveApi(meta, env);
  if (!err.ok() || !core_api_) {
    return err;
  }

  logger_->info("Using {} API", core_api_->GetApiMode());
}

std::string getRepoPath(CmdEnv& env) {
  std::string cgnt_home = std::getenv("CGNT_HOME");
  if (!cgnt_home.empty()) {
    return cgnt_home;
  }

  auto if_found =
      std::find_if(env.arguments.begin(), env.arguments.end(),
                   [](std::pair<std::string, std::string>& p) -> bool {
                     return (p.first == CONFIG_ARG);
                   });

  if (if_found != env.arguments.end()) {
    return if_found->second;
  }

  return common::utils::GetDefaultRepoPath();
}

Status Context::resolveApi(CmdMeta& meta) noexcept {
  // Check if cmd is disabled
  if (meta.IsNoLocal() && meta.IsNoRemote()) {
    return Status(StatusCode::FAILED,
                  "Command " + meta.GetName() + "was disabled");
  }

  // Can we just run this locally
  if (!meta.IsNoLocal()) {
    core_api_ = std::make_shared<CoreAPI>(new LocalApi(Core(config_)));
    return Status::OK;
  }

  if (!meta.IsNoRemote() || meta.IsRepoRequired()) {
    core_api_ = std::make_shared<CoreAPI>(new RemoteAPI(Core(config_)));
    return Status::OK;
  }
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
