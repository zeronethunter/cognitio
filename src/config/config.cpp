#include "config/config.hpp"

#include <google/protobuf/util/json_util.h>

#include <fstream>
#include <mutex>
#include <utility>

#include "common/status.hpp"

namespace cognitio {
namespace config {

bool Config::isConfigCreated(const std::string &path) const noexcept {
  return std::filesystem::exists(std::filesystem::path(path) / "config.json");
}

Config::Config(std::string repo_path) noexcept
    : repo_path_(std::move(repo_path)) {
  logger_ = common::logger::createLogger("Config logger");
}

Status Config::createConfig(const std::string &repo_path,
                            const std::string &api_address,
                            const std::string &dht_address,
                            const std::string &bootstrap_node_address,
                            const std::string &gc_time,
                            const std::string &gc_size) const noexcept {
  logger_->debug("Making config.");
  std::filesystem::path config_path(repo_path);
  config_path /= "config.json";

  //  std::filesystem::create_directory(repo_path);

  ProtoConfig config;
  config.set_repo_path(repo_path);
  config.set_api_address(api_address);
  config.set_dht_address(dht_address);
  config.set_bootstrap_node_address(bootstrap_node_address);
  config.set_gc_time(gc_time);
  config.set_gc_size(gc_size);

  std::fstream config_file(config_path.string(),
                           std::ios::out | std::ios::trunc);

  if (!config_file.is_open()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  std::string json_config;
  if (!google::protobuf::util::MessageToJsonString(config, &json_config).ok()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  config_file.write(json_config.c_str(),
                    static_cast<long>(json_config.length()));

  logger_->debug("Config successfully created, path: {}", config_path.string());
  return Status::OK;
}

Status Config::getExistedConfig(const std::string &path) noexcept {
  logger_->debug("Getting existed config.");

  std::fstream config_file(path, std::ios::in);

  if (!config_file.is_open()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  std::string json_config =
      std::string((std::istreambuf_iterator<char>(config_file)),
                  std::istreambuf_iterator<char>());
  ProtoConfig config;

  if (!google::protobuf::util::JsonStringToMessage(json_config, &config).ok()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  repo_path_ = config.repo_path();
  api_address_ = config.api_address();
  dht_address_ = config.dht_address();
  bootstrap_node_address_ = config.bootstrap_node_address();
  gc_size_ = config.gc_size();
  gc_time_ = config.gc_time();

  logger_->debug("Successfully got config");
  return Status::OK;
}

Status Config::TryInit() noexcept {
  logger_->debug("Trying to init config.");

  std::filesystem::path config_path(repo_path_);
  config_path /= "config.json";

  if (isConfigCreated(repo_path_)) {
    logger_->info("Found config in {}", repo_path_);
    return getExistedConfig(config_path.string());
  }

  return Status::FAILED;
}

bool Config::initialized() const noexcept {
  return !repo_path_.empty() && !api_address_.empty();
}

Status Config::Dump() const noexcept {
  logger_->debug("Dumping config in {}", repo_path_);

  if (initialized()) {
    logger_->info("Found config in {}", repo_path_);
    return createConfig(repo_path_, api_address_);
  }
  return createConfig(repo_path_);
}

std::string Config::Get(const std::string &field) const noexcept {
  if (field == "repo_path") {
    return repo_path_;
  }

  if (field == "api_address") {
    return api_address_;
  }

  if (field == "dht_address") {
    return dht_address_;
  }

  if (field == "bootstrap_node_address") {
    return bootstrap_node_address_;
  }

  if (field == "gc_time") {
    return gc_time_;
  }

  if (field == "gc_size") {
    return gc_size_;
  }

  return {};
}

void Config::SetRepoPath(const std::string &repo_path) noexcept {
  repo_path_ = repo_path;
}

// Static members
std::once_flag Config::flag_;
std::shared_ptr<Config> Config::instance_ = nullptr;

}  // namespace config
}  // namespace cognitio
