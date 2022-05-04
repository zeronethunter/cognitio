// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_CONTEXT_HPP_
#define CGNT_CORE_COMMANDS_CONTEXT_HPP_

#include <grpcpp/impl/codegen/core_codegen.h>

#include "core/core.hpp"
#include "core/core_api/core_api.hpp"
#include "repo/config.hpp"

namespace cognitio {
namespace core {
namespace commands {
class Context {
 public:
  Context() = default;
  ~Context() = default;

  void SetConfig(repo::config::Config& conf);
  void SetAPI(core::core_api::CoreAPI& api);
  repo::config::Config& GetConfig() noexcept;
  core::core_api::CoreAPI& GetAPI() noexcept;
  core::Core& GetCore() noexcept;

 private:
  repo::config::Config config_;
  core::core_api::CoreAPI core_api_;
};
}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_CONTEXT_HPP_
