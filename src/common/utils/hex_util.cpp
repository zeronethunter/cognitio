// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/utils/hex_util.hpp"

#include <ios>

namespace cognitio {
namespace common {

std::string hex_upper(const std::span<uint8_t> &bytes) {
  std::string res(bytes.size() * 2, '\x00');
  boost::algorithm::hex(bytes.begin(), bytes.end(), res.begin());
  return res;
}

std::string hex_lower(const std::span<uint8_t> &bytes) {
  std::string res(bytes.size() * 2, '\x00');
  boost::algorithm::hex_lower(bytes.begin(), bytes.end(), res.begin());
  return res;
}

std::vector<uint8_t> unhex(const std::string_view &hex) {
  std::vector<uint8_t> blob;
  blob.reserve((hex.size() + 1) / 2);
  boost::algorithm::unhex(hex.begin(), hex.end(), std::back_inserter(blob));
  return blob;
}

}  // namespace common
}  // namespace cognitio
