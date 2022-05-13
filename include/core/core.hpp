// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_HPP_
#define CGNT_CORE_CORE_HPP_

#include <memory>

#include "config/config.hpp"
#include "exchange/block_service/block_service.hpp"

namespace cognitio {
namespace core {
class Core {
 public:
  typedef std::shared_ptr<Config> CfgPtr;

private :
  CfgPtr config_;
};
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_HPP_
