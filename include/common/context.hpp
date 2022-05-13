// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_CONTEXT_HPP_
#define CGNT_CORE_COMMANDS_CONTEXT_HPP_

#include <memory>

#include "cli/commands/command.hpp"
#include "core/core.hpp"
#include "core/core_api/core_api.hpp"
#include "repo/config.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace repo::config;
using namespace core::core_api;
using namespace cli::commands;

class Context {
 public:
  typedef std::shared_ptr<Config> CfgPtr;
  typedef std::shared_ptr<CoreAPI> ApiPtr;

  Context() = default;
  Context(Context& ctx) = delete;
  Context& operator=(Context& ctx) = delete;
  ~Context() = default;

  bool IsInitialized() const noexcept { return config_ && core_api_; }
  CfgPtr GetConfig() noexcept { return config_; }
  ApiPtr GetAPI() noexcept { return core_api_; }
  void SetConfig(Config&& conf) noexcept;
  void SetAPI(CoreAPI&& api) noexcept;

  Status Init(CmdMeta& meta, CmdEnv env) noexcept;

 private:
  CfgPtr config_ = nullptr;
  ApiPtr core_api_ = nullptr;
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_CONTEXT_HPP_
