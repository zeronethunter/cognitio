// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_HASHER_HPP_
#define CGNT_COMMON_HASHER_HPP_

#include <map>

#include "common/multiformats/multihash.hpp"

namespace cognitio {
namespace common {

class Hasher {
 public:
  Multihash Calculate(HashType hash_type, const std::span<uint8_t> &bytes);

  Multihash Sha2_256(std::span<uint8_t> &bytes);

 private:
  std::map<HashType, Multihash (*)(std::span<uint8_t>)> methods_;
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_HASHER_HPP_
