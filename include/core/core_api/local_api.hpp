// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_API_LOCAL_API_HPP_
#define CGNT_CORE_CORE_API_LOCAL_API_HPP_

#include <memory>

#include "cli/commands/response_emitter.hpp"
#include "common/multiformats/cid.hpp"
#include "core/core.hpp"
#include "core/core_api/core_api.hpp"

namespace cognitio {
namespace core {
namespace core_api {

using namespace cli::commands;

class LocalAPI : public CoreAPI {
 public:
  explicit LocalAPI(Core&& core) : CoreAPI(std::move(core)) {}
  void Remove(const common::Cid& cid, ResponseEmitter& re) override;
  void Get(const common::Cid& cid, ResponseEmitter& re) override;
  void Add(const std::string& path, ResponseEmitter& re) override;
};

}  // namespace core_api
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_API_LOCAL_API_HPP_
