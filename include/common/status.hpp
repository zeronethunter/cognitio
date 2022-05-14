// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_STATUS_HPP_
#define CGNT_COMMON_STATUS_HPP_

#include <string>

#include "common/status_code.hpp"

namespace cognitio {

class Status {
 public:
  Status() : code_(StatusCode::OK) {}

  Status(StatusCode code, const std::string& error_message = "")
      : code_(code), error_message_(error_message) {}

  Status(StatusCode code, const std::string& error_message,
         const std::string& error_details)
      : code_(code),
        error_message_(error_message),
        binary_error_details_(error_details) {}

  // Pre-defined special status objects.
  static const Status& OK;
  static const Status& FAILED;

  StatusCode error_code() const { return code_; }
  std::string error_message() const { return error_message_; }
  std::string error_details() const { return binary_error_details_; }

  bool ok() const { return code_ == StatusCode::OK; }

 private:
  StatusCode code_;
  std::string error_message_;
  std::string binary_error_details_;
};

}  // namespace cognitio

#endif  // CGNT_COMMON_STATUS_HPP_
