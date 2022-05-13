// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_
#define CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_

#include <cstdlib>
#include <span>
#include <string>
#include <vector>

#include "common/status.hpp"
#include "common/utils/hex_util.hpp"
#include "SHA256.h"

namespace cognitio {
namespace common {

//! Data of Multihash
class Multihash {
 public:
  Multihash();

  Multihash(const Multihash &other) = default;
  Multihash &operator=(const Multihash &other);
  Multihash(Multihash &&other) noexcept = default;
  Multihash &operator=(Multihash &&other) noexcept;

  ~Multihash() = default;

  Status CreateFromHex(std::string_view &hex);

  Status CreateFromBytes(std::span<uint8_t> &bytes);

  void ToHash(const std::span<uint8_t> &bytes);

  //! \return info about hash type
  const std::span<uint8_t> GetHash() const {
    return std::span<uint8_t>(data().bytes_).subspan(data().hash_offset_);
  };

  //! \return buffer with the multihash
  const std::vector<uint8_t> ToBuffer() const { return data().bytes_; };

  //! \return string view of multihash
  std::string ToHex() const { return hex_upper(data().bytes_); };

  //! Overloading of comparison operators
  bool operator==(const Multihash &other);
  bool operator!=(const Multihash &other);
  bool operator<(const Multihash &other);

 private:
  const uint8_t max_hash_length_ = 127;
  const uint8_t min_hash_length_ = 4;

  struct Data {
    std::vector<uint8_t> bytes_;
    uint8_t hash_offset_;  //! size of non-hash data from the beginning
    size_t hash_size_;

    Data(std::span<const uint8_t> hash);
  };

  std::shared_ptr<Data> data_;

  Data &data() const { return *data_; };
  Multihash(const std::span<uint8_t> &hash)
      : data_(std::make_shared<Data>(hash)){};
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_MILTIFORMATS_MULTIHASH_HPP_
