//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CONFIG_CONFIG_HPP_
#define CGNT_CONFIG_CONFIG_HPP_

#include <filesystem>
#include <nlohmann/json.hpp>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "proto/config/ProtoConfig.pb.h"

namespace cognitio {
namespace config {

/**
 *  @brief  Config implementation.
 *
 *  Opens in root path. Dump and get json data.\n
 *
 *  Provides converting mechanism between Json and Protobuf.
 *
 */
class Config {
  Config() noexcept = delete;
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  /**
   *  @brief  Config constructor.
   *
   *  Add repo_path member of class.
   *
   *  @param repo_path path to repository.
   */
  Config(const std::string &repo_path) noexcept;

  /**
   *  @brief  Config creation / getting from existing sources.
   *
   *  If config is already exists in repo, get it. Create it overwise.
   */
  Status TryInit() noexcept;

  /**
   *  @brief  Config dumping in repo_path.
   */
  Status Dump() const noexcept;

  /**
   *  @brief  Get config field by key (name).
   *
   *  @param field name of field to get.
   */
  std::string Get(const std::string &field) const noexcept;

  /**
   *  @brief  Set repo path.
   */
  Status SetRepoPath(const std::string &repo_path) noexcept;
  /**
   *  @brief  Check if all of the config required fields are initialized.
   */
  bool Initialized() const noexcept;

 private:
  bool isConfigCreated(const std::string &path) const noexcept;

  Status getExistedConfig(const std::string &path) noexcept;
  Status createConfig(
      const std::string &repo_path,
      const std::string &api_address = "127.0.0.1::10000") const noexcept;

  std::string repo_path_;
  std::string api_address_;
  common::logger::Logger logger = common::logger::createLogger("Config logger");
};

}  // namespace config
}  // namespace cognitio

#endif  // CGNT_CONFIG_CONFIG_HPP_
