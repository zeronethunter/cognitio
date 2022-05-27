// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/context/context.hpp"

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <utility>

#include "cli/commands/command.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "config/config.hpp"
#include "core/core_api/local_api.hpp"
#include "core/core_api/remote_api.hpp"
#include "repo/repo.hpp"

#define CONFIG_ARG "--config"

namespace cognitio {
namespace core {
namespace commands {

Status Context::Init(CmdMeta& meta, CmdEnv& env) noexcept {
  repo_path_ = getRepoPath(env);
  logger_->debug("Checking repo path: {}", repo_path_);

  config_.SetRepoPath(repo_path_);
  auto err = config_.TryInit();

  if (meta.GetName() != "init") {
    if (!err.ok()) {
      logger_->error("Repo is not initialized. Use 'cognitio init'");
      return Status::FAILED;
    }

    if (config_.Get("repo_path").empty()) {
      config_.SetRepoPath(repo_path_);
    }
  }

  core_ = std::make_shared<Core>(repo_path_);
  err = resolveApi(meta, env);

  if (!err.ok() || !core_api_) {
    return err;
  }

  return Status::OK;
}

std::string Context::getRepoPath(CmdEnv& env) const noexcept {
  // TODO: it's not ok to hard code like that
  if (env.arguments.contains("--repo")) {
    return env.arguments.at("--repo");
  }

  auto cgnt_home = std::getenv("CGNT_HOME");
  if (cgnt_home) {
    return std::string(cgnt_home);
  }

  auto if_found =
      std::find_if(env.arguments.begin(), env.arguments.end(),
                   [](const std::pair<std::string, std::string>& p) -> bool {
                     return (p.first == CONFIG_ARG);
                   });

  if (if_found != env.arguments.end()) {
    return if_found->second;
  }

  return common::utils::GetDefaultRepoPath();
}

Status Context::resolveApi(CmdMeta& meta,
                           [[maybe_unused]] CmdEnv& env) noexcept {
  // Check if cmd is disabled
  if (meta.IsNoLocal() && meta.IsNoRemote()) {
    return Status(StatusCode::FAILED,
                  "Command " + meta.GetName() + "was disabled");
  }

  // We're always trying to run it remotely first
  if (!meta.IsNoRemote()) {
    std::string api_addr = resolveAddr(env);
    if (!api_addr.empty()) {
      auto temp_api = std::make_shared<RemoteAPI>(api_addr);
      logger_->info("Trying to connect to daemon with address {}", api_addr);
      auto err = temp_api->TryPing();
      if (err.ok()) {
        logger_->info("Using remote executor");
        core_api_ = temp_api;
        return Status::OK;
      }

      logger_->warn("Failed to connect. Falling back to local executor");
    }
  }

  // Now we just run it locally
  core_api_ = std::make_shared<LocalAPI>(core_);
  return Status::OK;
}

std::string Context::resolveAddr(CmdEnv& env) const noexcept {
  for (const auto& x : env.arguments) {
    if (x.first == "--api") {
      return x.second;
    }
  }

  return config_.Get("api_address");
}

}  // namespace commands
}  // namespace core
}  // namespace cognitio
