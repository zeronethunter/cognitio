// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_MULTIFORMATS_MULTIADDR_HPP
#define CGNT_COMMON_MULTIFORMATS_MULTIADDR_HPP

#include <optional>
#include <string>
#include <vector>

namespace cognitio {
namespace common {

class Multiaddress {
 public:
  
 private:
  std::string address_str_;
  std::vector<uint8_t> bytes_;
  std::optional<std::string> peer_id_;
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_MULTIFORMATS_MULTIADDR_HPP
