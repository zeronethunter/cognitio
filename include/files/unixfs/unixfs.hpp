// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_FILES_UNIXFS_UNIXFS_HPP_
#define CGNT_FILES_UNIXFS_UNIXFS_HPP_

#include "common/status.hpp"
#include "unixfs.pb.h"

namespace cognitio {
namespace files {
namespace unixfs {

/**
 *  @brief  Managing data blocks:
 *  changing, managing, encoding, decoding in protobuf.
 *
 *  @First you should create UnixFS by using special method CreateUnixFS.
 *  @Then use method EncodeMessage to make Data class member to throw.
 *  @Also you can change all of UnixFS message fields with help of getters and
 * setters
 */
class UnixFS {
 public:
  /**
   *  @brief  Encode a data to protobuf.
   *  @param  data vector of bytes.
   *
   *  @return an unique ptr to protobuf class Data.
   */
  std::unique_ptr<Data> EncodeMessage(const std::vector<uint8_t>& data);
  /**
   *  @brief  Decode from probuf to data.
   *  @param  encoded protobuf class Data.
   *
   *  @return a vector of bytes that was taken from protobuf.
   */
  static std::vector<uint8_t> DecodeMessage(const Data& encoded);
  /**
   *  @brief  Initialize protobuf message pattern.
   *  @param  datatype data type of future protobuf message. Example: "raw",
   * "directory", "file" e.t.c.
   *  @param  filesize file size in bytes.
   *  @param  blocksizes vector of block sizes.
   *
   *  @return a Status.
   */
  Status CreateUnixFS(const std::string& datatype, uint64_t filesize,
                      const std::vector<uint64_t>& blocksizes);
  /**
   *  @brief  Adder for blocksize.
   *  @param  size_in_bytes size in bytes to add.
   */
  void AddBlockSize(uint64_t size_in_bytes) {
    blocksizes_.push_back(size_in_bytes);
  }
  /**
   *  @brief  Get block size by index.
   *  @param  index index of size to get.
   *  @return size.
   */
  size_t GetBlockSize(size_t index) { return blocksizes_.at(index); }
  /**
   *  @brief  Get vector of block sizes.
   *  @return vector of block sizes.
   */
  std::vector<uint64_t> GetBlockSize() { return blocksizes_; }
  /**
   *  @brief  Get file size.
   */
  uint64_t GetFilesize() { return filesize_; }
  /**
   *  @brief  Set file size.
   *  @param  filesize size of file to set.
   */
  void SetFilesize(uint64_t filesize) { filesize_ = filesize; }
  /**
   *  @brief  Get type of data.
   */
  std::string GetDataType() { return data_type_; }

 private:
  bool is_created_ = false;
  std::string data_type_;
  std::vector<uint8_t> data_;
  uint64_t filesize_;
  std::vector<uint64_t> blocksizes_;
};

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio

#endif  // CGNT_FILES_UNIXFS_UNIXFS_HPP_
