// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/utils/repo.hpp"

namespace cognitio {
namespace common {
namespace utils {

std::string GetDefaultRepoPath() {
  auto repo_home_char = std::getenv("XDG_DATA_HOME");
  std::string repo_home;
  if (!repo_home_char) {
    auto home_tmp_char = std::getenv("HOME");
    if (!home_tmp_char) {
      return {};
    }
    repo_home = std::string(home_tmp_char) + "/.cognitio";
  } else {
    repo_home = std::string(repo_home_char) + "/cognitio";
  }

  return repo_home;
}

[[nodiscard]] std::pair<int, Status> ToBytes(const std::string &size) noexcept {
  std::string postfix = size.substr(size.length() - 2, 2);
  std::string prefix = size.substr(0, size.length() - 2);

  if (postfix == "Mb" || postfix == "mb") {
    return {std::stoi(prefix) * 1024 * 1024, Status::OK};
  }
  if (postfix == "Kb" || postfix == "kb") {
    return {std::stoi(prefix) * 1024, Status::OK};
  }
  int result;
  try {
    result = std::stoi(size);
  } catch (std::exception &e) {
    Status status(StatusCode::FAILED, "Wrong format of GC size:" + size +
                                          ". Only Gb, gb, Kb, kb or empty "
                                          "available.");
    return {result, status};
  }
  return {result, Status::OK};
}

[[nodiscard]] std::pair<std::chrono::duration<int64_t>, Status> ToTime(
    const std::string &time) noexcept {
  char postfix = time[time.length() - 1];
  std::string prefix = time.substr(0, time.length() - 1);
  int result;

  try {
    result = std::stoi(prefix);
  } catch (std::exception &e) {
    Status status(StatusCode::FAILED, "Wrong format of GC time:" + time +
                                          ". Only s, m, h available.");
    return {std::chrono::duration<int64_t>(), status};
  }

  switch (postfix) {
    case 's':
      return {std::chrono::seconds(result), Status::OK};
    case 'm':
      return {std::chrono::minutes(result), Status::OK};
    case 'h':
      return {std::chrono::hours(result), Status::OK};
    default:
      Status status(StatusCode::FAILED, "Wrong format of GC time:" + time +
                                            ". Only s, m, h available.");
      return {std::chrono::duration<int64_t>(), status};
  }
}

}  // namespace utils
}  // namespace common
}  // namespace cognitio
