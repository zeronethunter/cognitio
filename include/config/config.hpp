//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CONFIG_CONFIG_HPP_
#define CGNT_CONFIG_CONFIG_HPP_

#include <filesystem>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "proto/ProtoConfig.pb.h"

namespace cognitio {
namespace config {

class Config {
 public:
  Config() noexcept = delete;
  // Config(const Config &) = delete;
  // Config &operator=(const Config &) = delete;
  explicit Config(const std::string &repo_path) noexcept;

  Status TryInit() noexcept;

  [[nodiscard]] Status Dump() const noexcept;

  [[nodiscard]] std::string Get(const std::string &field) const noexcept;

  Status SetRepoPath(const std::string &repo_path) noexcept;

 private:
  [[nodiscard]] bool isConfigCreated(const std::string &path) const noexcept;

  Status getExistedConfig(const std::string &path) noexcept;
  [[nodiscard]] Status createConfig(
      const std::string &repo_path,
      const std::string &api_address = "127.0.0.1::10000") const noexcept;
  [[nodiscard]] bool initialized() const noexcept;

 private:
  std::string repo_path_;
  std::string api_address_;
  common::logger::Logger logger = common::logger::createLogger("Config logger");
};

}  // namespace config
}  // namespace cognitio

#endif  // CGNT_CONFIG_CONFIG_HPP_
