// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_UTILS_FILESYSTEM_HPP_
#define CGNT_COMMON_UTILS_FILESYSTEM_HPP_

#include <utility>
#include <vector>

#include "common/status.hpp"

namespace cognitio {
namespace common {
namespace utils {

std::pair<std::vector<uint8_t>, Status> get_bytes_from(const std::string& path);

}  // namespace utils
}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_UTILS_FILESYSTEM_HPP_
