// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_API_HPP_
#define CGNT_CORE_CORE_API_HPP_

#include <memory>

#include "cli/commands/response_emitter.hpp"
#include "core/core.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace core {
namespace core_api {

using namespace cli::commands;

class CoreAPI {
 public:
  CoreAPI() = default;
  CoreAPI(CoreAPI& api) = delete;
  CoreAPI& operator=(CoreAPI& api) = delete;
  ~CoreAPI() = default;

  virtual void Remove(const common::Cid& cid, ResponseEmitter& re) = 0;
  virtual void Get(const common::Cid& cid, ResponseEmitter& re) = 0;
  virtual void Add(const std::string& path, ResponseEmitter& re) = 0;
};

}  // namespace core_api
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_API_HPP_
