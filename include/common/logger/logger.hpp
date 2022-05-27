// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_LOGGER_LOGGER_HPP_
#define CGNT_COMMON_LOGGER_LOGGER_HPP_

#include "spdlog/spdlog.h"

namespace cognitio {
namespace common {
namespace logger {

using Logger = std::shared_ptr<spdlog::logger>;

//! \brief Provide logger object
//! \param tag - tagging name for identifying logger
//! \return logger object
Logger createLogger(const std::string& tag, bool debug_mode = true);

}  // namespace logger
}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_LOGGER_LOGGER_HPP_
