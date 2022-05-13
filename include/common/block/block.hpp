// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_BLOCK_BLOCK_HPP
#define CGNT_COMMON_BLOCK_BLOCK_HPP

#include <string>
#include <vector>

#include "common/multiformats/cid.hpp"

namespace cognitio {
namespace block {

//! A BasicBlock is a singular block of data in ipfs.
class Block 
{
 public:
  Block() = default;

  //! Creates Block on bytes view
  Block(const std::vector<uint8_t> &bytes);

  //! NewBlock creates a Block object from opaque data. It will hash the data.
  Block(const std::vector<uint8_t> &bytes, const cognitio::common::Cid &cid);

  //! GetRawData returns bytes view of block
  std::vector<uint8_t> GetRawData() const;

  //! GetCid returns Cid of block
  cognitio::common::Cid GetCid() const;

  //! GetString returns string view of block
  std::string GetString() const;

  //! Multihash returns the hash contained in the block CID.
  common::Multihash Multihash() const;

 private:
  cognitio::common::Cid cid_;      //! Cid of block
  std::vector<uint8_t> bytes_;  //! bytes of block
};

}  // namespace block
}  // namespace cognitio

#endif  // CGNT_COMMON_BLOCK_BLOCK_HPP
