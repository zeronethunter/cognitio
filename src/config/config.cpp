#include "config/config.hpp"

#include <google/protobuf/util/json_util.h>

#include <fstream>
#include "common/status.hpp"

namespace cognitio {
namespace config {

bool Config::isConfigCreated(const std::string &path) const noexcept {
  return std::filesystem::exists(std::filesystem::path(path) / "config");
}

Config::Config(const std::string &repo_path) noexcept : repo_path_(repo_path) {
  logger_ = common::logger::createLogger("Config logger");
}

Status Config::createConfig(const std::string &repo_path,
                            const std::string &api_address) const noexcept {
  std::filesystem::path config_path(repo_path);
  config_path /= "config";

  ProtoConfig config;
  config.set_repo_path(repo_path);
  config.set_api_adress(api_address);

  std::fstream config_file(config_path.string(), std::ios::out);

  if (!config_file.is_open()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  std::string json_config;
  if (!google::protobuf::util::MessageToJsonString(config, &json_config).ok()) {
    return {StatusCode::FAILED, "Failed to open config file."};
  }

  config_file.write(json_config.c_str(),
                    static_cast<long>(json_config.length()));

  return Status::OK;
}

Status Config::getExistedConfig(const std::string &path) noexcept {
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

  return Status::OK;
}

Status Config::TryInit() noexcept {
  std::filesystem::path config_path(repo_path_);
  config_path /= "config";

  if (isConfigCreated(repo_path_)) {
    return getExistedConfig(config_path.string());
  }

  return Status::FAILED;
}

bool Config::initialized() const noexcept {
  return !repo_path_.empty() && !api_address_.empty();
}

Status Config::Dump() const noexcept {
  if (initialized()) {
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
  return {};
}

void Config::SetRepoPath(const std::string &repo_path) noexcept {
  repo_path_ = repo_path;
}

}  // namespace config
}  // namespace cognitio
