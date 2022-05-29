// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_UTILS_REPO_HPP_
#define CGNT_COMMON_UTILS_REPO_HPP_

#include <chrono>
#include <cstdlib>
#include <string>

#include "common/status.hpp"

namespace cognitio {
namespace common {
namespace utils {
std::pair<int, Status> ToBytes(const std::string &size) noexcept;
[[nodiscard]] std::pair<std::chrono::duration<int64_t>, Status> ToTime(
    const std::string &time) noexcept;
std::string GetDefaultRepoPath();

}  // namespace utils
}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_UTILS_REPO_HPP_
