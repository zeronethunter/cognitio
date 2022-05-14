// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef COGNT_COMMON_UTILS_HEX_UTIL_HPP_
#define COGNT_COMMON_UTILS_HEX_UTIL_HPP_

#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/hex.hpp>

#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace cognitio {
namespace common {

/// @brief converts bytes to uppercase hex view
/// @param array of bytes
/// @return hex string
std::string hex_upper(const std::span<uint8_t> &bytes);

/// @brief converts bytes to lowercase hex view
/// @param array of bytes
/// @return hex string
std::string hex_lower(const std::span<uint8_t> &bytes);

/// \brief Converts hex representation to bytes view
/// \param Hex string
/// \return vector of bytes
std::vector<uint8_t> unhex(const std::string_view &hex_str);

}  // namespace common
}  // namespace cognitio

#endif  // COGNT_COMMON_UTILS_HEX_UTIL_HPP_
