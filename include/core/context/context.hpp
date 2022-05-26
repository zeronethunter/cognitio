// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_CONTEXT_HPP_
#define CGNT_CORE_COMMANDS_CONTEXT_HPP_

#include <memory>
#include <string>

#include "cli/commands/command.hpp"
#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "config/config.hpp"
#include "core/core.hpp"
#include "core/core_api/core_api.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace core::core_api;
using namespace cli::commands;
using namespace common::logger;

class Context {
 public:
  typedef std::shared_ptr<Config> CfgPtr;
  typedef std::shared_ptr<CoreAPI> ApiPtr;
  typedef std::shared_ptr<Core> CorePtr;

  Context() = default;
  Context(Context& ctx) = delete;
  Context& operator=(Context& ctx) = delete;
  ~Context() = default;

  bool IsInitialized() const noexcept { return core_api_.get(); }
  const std::string& GetRepoPath() const noexcept { return repo_path_; }

  Config& GetConfig() noexcept { return config_; }
  CorePtr GetCore() noexcept { return core_; }
  ApiPtr GetAPI() noexcept { return core_api_; }
  Status Init(CmdMeta& meta, CmdEnv& env) noexcept;

 private:
  std::string getRepoPath(CmdEnv& env) const noexcept;
  Status resolveApi(CmdMeta& meta, CmdEnv& env) noexcept;
  std::string resolveAddr(CmdEnv& env) const noexcept;

  CorePtr core_ = nullptr;
  ApiPtr core_api_ = nullptr;
  std::string repo_path_ = "";
  Logger logger_ = createLogger("context");
  Config& config_ = Config::GetInstance();
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_CONTEXT_HPP_
