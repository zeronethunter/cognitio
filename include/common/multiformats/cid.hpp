// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>
#ifndef CGNT_COMMON_MILTIFORMATS_CID_HPP_
#define CGNT_COMMON_MILTIFORMATS_CID_HPP_

#ifndef CGNT_UTILS_MILTIFORMATS_CID_HPP_
#define CGNT_UTILS_MILTIFORMATS_CID_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "linked_data/node.hpp"

namespace cognitio {
namespace cid {

//! Prefix represents all the metadata of a Cid,
//! that is, the Version, the Codec, the Multihash type
//! and the Multihash length. It does not contains
//! any actual content information.
struct Prefix {
  uint64_t version_;
  uint64_t codec_;
  uint64_t mh_type_;
  int mh_length_;

  Prefix() = default;
  ~Prefix() = default;

  Prefix(uint64_t version, uint64_t codec, uint64_t mh_type, int mh_length)
      : version_(version),
        codec_(codec),
        mh_type_(mh_type),
        mh_length_(mh_length){};
};

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

//! Content identifier.
//! View: <multicodec><multihash-algorithm><multihash-length><multihash-hash>
class Cid {
 public:
  Cid();

  //! Creates a new Cid.
  Cid(const cognitio::linked_data::Node &node);

  Cid(const Cid &cid) = default;

  Cid(Cid &&cid);

  //! Equals operator checks that two Cids are the same.
  bool operator==(const cognitio::cid::Cid &other);

  //! Equals operator checks that two Cids are not the same.
  bool operator!=(const cognitio::cid::Cid &other);

  //! GetBytes returns the byte representation of a Cid.
  std::vector<uint8_t> GetBytes() const;

  //! GetPrefix returns prefix form of Cid.
  std::string GetPrefix() const;

  //! ToString returns the default string representation of a Cid.
  std::string ToString() const { return str_cid_; };

  //! Type returns the multicodec-packed content type of a Cid.
  uint64_t GetType() const;

 private:
  std::string str_cid_;                //! string view of cid.
  std::vector<uint64_t> bytes_view_;   //! bytes array view of cid.
  cognitio::cid::Prefix prefix_form_;  //! contains version, codec,
                                       //! mh-type and -length.
  cognitio::cid::CodeType content_type_;
};

}  // namespace cid
}  // namespace cognitio

#endif  // CGNT_UTILS_MILTIFORMATS_CID_HPP_
