// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/utils/filesystem.hpp"

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

#include "common/status.hpp"
#include "common/status_code.hpp"

namespace cognitio {
namespace common {
namespace utils {

std::pair<std::vector<uint8_t>, Status> get_bytes_from(
    const std::string& path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    return std::make_pair(std::vector<uint8_t>(),
                          Status(StatusCode::FAILED, "Unable to open file"));
  }

  std::stringstream buffer_str;
  buffer_str << file.rdbuf();
  std::string buf_str = buffer_str.str();
  std::vector<uint8_t> buffer(buf_str.begin(), buf_str.end());
  return std::make_pair(buffer, Status::OK);
}

}  // namespace utils
}  // namespace common
}  // namespace cognitio
