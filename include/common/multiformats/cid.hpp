// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "linked_data/node.hpp"

#include <cstddef>
#include <iostream>
#include <vector>

#ifndef CGNT_COMMON_MILTIFORMATS_CID_HPP
#define CGNT_COMMON_MILTIFORMATS_CID_HPP

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

  Prefix(uint64_t version, 
         uint64_t codec,
         uint64_t mh_type,
         int mh_length) : version_(version), 
                          codec_(codec), 
                          mh_type_(mh_type), 
                          mh_length_(mh_length) 
                          {};
};

//! Content identifier.
//! View: <multicodec><multihash-algorithm><multihash-length><multihash-hash>
class Cid {
 public:
  Cid() = default;
  
  //! Creates a new Cid.
  Cid(const cognitio::linked_data::Node &node);

  //! Equals operator checks that two Cids are the same.
  bool operator==(const cognitio::cid::Cid &other);

  //! Equals operator checks that two Cids are not the same.
  bool operator!=(const cognitio::cid::Cid &other);

  //! GetBytes returns the byte representation of a Cid.
  std::vector<uint8_t> GetBytes() const;
  
  //! GetPrefix returns prefix form of Cid.
  std::string GetPrefix() const;

  //! ToString returns the default string representation of a Cid.
  std::string ToString() const;

  //! Type returns the multicodec-packed content type of a Cid.
  uint64_t GetType() const;

 private:
  std::string str_cid_;                //! string view of cid.

  std::vector<uint64_t> bytes_view_;    //! bytes array view of cid.

  cognitio::cid::Prefix prefix_form_;  //! contains version, codec,
                                       //! mh-type and -length.
};

}  // namespace cid
}  // namespace cognitio

#endif  // CGNT_COMMON_MILTIFORMATS_CID_HPP
