// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_API_HPP_
#define CGNT_CORE_CORE_API_HPP_

#include <memory>

#include "cli/commands/response_emitter.hpp"
#include "common/multiformats/cid.hpp"
#include "core/core.hpp"

namespace cognitio {
namespace core {
namespace core_api {

using namespace cli::commands;

class CoreAPI {
 public:
  typedef std::unique_ptr<Core> CorePtr;

  explicit CoreAPI(Core&& core)
      : core_(std::make_unique<Core>(std::move(core))) {}

  virtual void Remove(const common::Cid& cid, ResponseEmitter& re) = 0;
  virtual void Get(const common::Cid& cid, ResponseEmitter& re) = 0;
  virtual void Add(const std::string& path, ResponseEmitter& re) = 0;

  CorePtr GetCore() noexcept { return core_; }

 private:
  std::unique_ptr<Core> core_;
};

}  // namespace core_api
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_API_HPP_
