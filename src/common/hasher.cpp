// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/hasher.hpp"

namespace cognitio {
namespace common {

using HashMethod = Multihash (*)(std::span<uint8_t>);

Multihash Hasher::Calculate(HashType hash_type,
                            const std::span<uint8_t> &bytes) {
  HashMethod hash_method = methods_.at(hash_type);
  return std::invoke(hash_method, bytes);
}

Multihash Hasher::Sha2_256(std::span<uint8_t> &bytes) {
    
}

}  // namespace common
}  // namespace cognitio
