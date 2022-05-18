// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/status.hpp"

namespace cognitio {

const Status Status::OK = Status(StatusCode::OK);
const Status Status::FAILED = Status(StatusCode::FAILED);

}  // namespace cognitio
