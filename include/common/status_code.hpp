// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_STATUS_CODE_HPP_
#define CGNT_COMMON_STATUS_CODE_HPP_

namespace cognitio {

enum StatusCode {
  OK = 0,
  FAILED = 1,
  CANCELLED = 2,
  UNKNOWN = 3,
  INVALID_ARGUMENT = 4,
  NOT_FOUND = 5,
  ALREADY_EXISTS = 6,
  PERMISSION_DENIED = 7,
  OUT_OF_RANGE = 8,
  UNIMPLEMENTED = 9,
  INTERNAL = 10,
};

}  // namespace cognitio

#endif  // CGNT_COMMON_STATUS_CODE_HPP_
