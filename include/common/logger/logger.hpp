// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_LOGGER_LOGGER_HPP_
#define CGNT_COMMON_LOGGER_LOGGER_HPP_

#include <glog/logging.h>

namespace cognitio {
namespace common {
namespace logger {

void LogInit();
void LogSetSeverity(int argc, char *argv[]);

}
}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_LOGGER_LOGGER_HPP_
