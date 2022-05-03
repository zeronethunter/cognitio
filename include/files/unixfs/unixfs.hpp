// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/status.hpp"
#include "unixfs.pb.h"

#ifndef CGNT_FILES_UNIXFS_UNIXFS_HPP_
#define CGNT_FILES_UNIXFS_UNIXFS_HPP_

namespace cognitio {
namespace files {
namespace unixfs {

class UnixFS {
  //!\brief Managing data blocks:
  //!\brief changing, managing, encoding, decoding in Protobuf

 public:
  std::unique_ptr<Data> EncodeMessage(
      const std::vector<uint8_t>& data);  //! Encode data to probuf
  static std::vector<uint8_t> DecodeMessage(
      const Data& encoded);  //! Decode from probuf to data
  Status CreateUnixFS(const std::string& datatype, uint64_t filesize,
                      const std::vector<uint64_t>&
                          blocksizes);  //! Initialize protobuf message pattern
  void AddBlockSize(uint64_t size_in_bytes) {
    blocksizes_.push_back(size_in_bytes);
  }  //! Adder for blocksize
  size_t GetBlockSize(size_t index) {
    return blocksizes_.at(index);
  }  //! Get blocksize by index
  std::vector<uint64_t> GetBlockSize() {
    return blocksizes_;
  }  //! Get vector of blocksizes
  uint64_t GetFilesize() { return filesize_; }
  void SetFilesize(uint64_t filesize) { filesize_ = filesize; }
  std::string GetDataType() { return data_type_; }

 private:
  std::string data_type_;
  std::vector<uint8_t> data_;
  uint64_t filesize_;
  std::vector<uint64_t> blocksizes_;
};

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio

#endif  // CGNT_FILES_UNIXFS_UNIXFS_HPP_
