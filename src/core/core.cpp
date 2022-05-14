// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core.hpp"

#include <memory>

#include "common/status.hpp"
#include "common/status_code.hpp"
#include "exchange/block_service/block_service.hpp"

namespace cognitio {
namespace core {

Status Core::RunDaemon() noexcept { return Status(StatusCode::UNIMPLEMENTED); }

}  // namespace core
}  // namespace cognitio
