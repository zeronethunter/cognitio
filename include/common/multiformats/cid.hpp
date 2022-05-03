// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_MILTIFORMATS_CID_HPP_
#define CGNT_COMMON_MILTIFORMATS_CID_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "common/multiformats/multihash.hpp"
#include "linked_data/node.hpp"

namespace cognitio {
namespace common {

enum CodeType {
  IDENTITY = 0x00,
  SHA1 = 0x11,
  SHA2_256 = 0x12,
  SHA2_512 = 0x13,
  SHA3_512 = 0x14,
  SHA3_384 = 0x15,
  SHA3_256 = 0x16,
  SHA3_224 = 0x17,
  RAW = 0x55,
  DAG_PB = 0x70,
  DAG_CBOR = 0x71,
  FILECOIN_COMMITMENT_UNSEALED = 0xf101,
  FILECOIN_COMMITMENT_SEALED = 0xf102,
};

//* Content identifier.
//* View: <multicodec><multihash-algorithm><multihash-length><multihash-hash>
class Cid {
 public:
  Cid();

  Cid(std::span<uint8_t> &bytes);
  Cid(std::string_view &str_view);
  Cid(CodeType code_type, const Multihash &content_address);

  Cid(const Cid &other);

  Cid(Cid &&other);

  Cid &operator=(const Cid &other);

  Cid &operator=(Cid &&other);

  //! Equals operator checks that two Cids are the same.
  bool operator==(const Cid &other);

  //! Equals operator checks that two Cids are not the same.
  bool operator!=(const Cid &other);

  //! GetBytes returns the byte representation of a Cid.
  std::vector<uint8_t> GetBytes() const { return bytes_view_; };

  //! ToString returns the default string representation of a Cid.
  std::string ToString() const { return str_cid_; };

  //! Type returns the multicodec-packed content type of a Cid.
  uint64_t GetType() const { return content_type_; };

 private:
  std::string str_cid_;              //! string view of cid.
  std::vector<uint8_t> bytes_view_;  //! bytes array view of cid.
  cognitio::common::Multihash content_address_;
  CodeType content_type_;
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_UTILS_MILTIFORMATS_CID_HPP_
