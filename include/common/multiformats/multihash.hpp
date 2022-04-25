// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <vector>
#include <span>

#ifndef CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_
#define CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_

namespace cognitio {
namespace multihash {

enum HashType {
    IDENTITY = 0x0,
    SHA1 = 0x11,
    SHA256 = 0x12,
    SHA512 = 0x13,
    BLAKE2B_256 = 0xb220,
    BLAKE2S128 = 0xb250,
    BLACKE2S256 = 0xb260,
    SHA2_256_TRUNC254_PADDED = 0x1012,
};


//! Data of Multihash
struct Data {
    std::vector<uint8_t> bytes_;
    uint8_t hash_offset_;           //! size of non-hash data from the beginning
    HashType hash_type_;
    size_t hash_size_;

    Data(HashType ht, std::span<const uint8_t> hash);
};

class Multihash {
 private:
};

}  // namespace multihash
}  // namespace cognitio

#endif  // CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_
