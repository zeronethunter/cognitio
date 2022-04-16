// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_STATUS_CODE_HPP_
#define CGNT_COMMON_STATUS_CODE_HPP_

namespace cognitio {

enum StatusCode {
  OK = 0,
  CANCELLED = 1,
  UNKNOWN = 2,
  INVALID_ARGUMENT = 3,
  NOT_FOUND = 4,
  ALREADY_EXISTS = 5,
  PERMISSION_DENIED = 6,
  OUT_OF_RANGE = 7,
  UNIMPLEMENTED = 8,
  INTERNAL = 9,
};

}  // namespace cognitio

#endif  // CGNT_COMMON_STATUS_CODE_HPP_
