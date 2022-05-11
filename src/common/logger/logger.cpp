// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/logger/logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace {
void setGlobalPattern(spdlog::logger& logger) {
  logger.set_pattern("[%Y-%m-%d %H:%M:%S.%F] %n %v");
}

void setDebugPattern(spdlog::logger& logger) {
  logger.set_pattern("[%Y-%m-%d %H:%M:%S.%F][th:%t][%l][%@] %n %v");
}

std::shared_ptr<spdlog::logger> createLogger(const std::string& tag,
                                             bool debug_mode) {
  auto logger = spdlog::stdout_color_mt(tag);
  if (debug_mode) {
    setDebugPattern(*logger);
  } else {
    setGlobalPattern(*logger);
  }

  return logger;
}
}  // namespace

namespace cognitio {
namespace common {
namespace logger {

Logger createLogger(const std::string& tag, bool debug_mode) {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock(mutex);
  auto logger = spdlog::get(tag);
  if (logger == nullptr) {
    logger = ::createLogger(tag, debug_mode);
  }

  return logger;
}

}  // namespace logger
}  // namespace common
}  // namespace cognitio
