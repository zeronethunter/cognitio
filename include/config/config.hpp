//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CONFIG_CONFIG_HPP_
#define CGNT_CONFIG_CONFIG_HPP_

#include <filesystem>
#include <memory>
#include <mutex>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "proto/ProtoConfig.pb.h"

namespace cognitio {
namespace config {

class Config {
  Config() noexcept = default;
  explicit Config(std::string repo_path) noexcept;
  Config(const Config &conf)
      : repo_path_(conf.repo_path_), api_address_(conf.api_address_) {
    instance_ = conf.instance_;
    logger_ = common::logger::createLogger("config");
  }

  Config &operator=(const Config &conf) = delete;

 public:
  static Config &GetInstance() noexcept {
    std::call_once(flag_, []() {
      Config::instance_.reset(new Config());
    });

    return *Config::instance_;
  }

  // this one will create a new instalce of Cofnig by
  // copying the state from the old one. The lifetime of the
  // new instance wil be managed by shared_ptr
  static std::shared_ptr<Config> getForkedInstance() {
    auto s = new Config(Config::GetInstance());
    std::shared_ptr<Config> forked_instance;
    forked_instance.reset(s);
    return forked_instance;
  }

  Status TryInit() noexcept;
  [[nodiscard]] Status Dump() const noexcept;
  [[nodiscard]] std::string Get(const std::string &field) const noexcept;
  void SetRepoPath(const std::string &repo_path) noexcept;

 private:
  Status getExistedConfig(const std::string &path) noexcept;
  [[nodiscard]] Status createConfig(
      const std::string &repo_path,
      const std::string &api_address = "127.0.0.1:50050") const noexcept;
  [[nodiscard]] bool initialized() const noexcept;
  [[nodiscard]] bool isConfigCreated(const std::string &path) const noexcept;

  static std::once_flag flag_;
  static std::shared_ptr<Config> instance_;

  std::string repo_path_;
  std::string api_address_;
  common::logger::Logger logger_ = common::logger::createLogger("config");
};

}  // namespace config
}  // namespace cognitio

#endif  // CGNT_CONFIG_CONFIG_HPP_
