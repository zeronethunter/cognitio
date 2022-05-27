// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_FILES_UNIXFS_UNIXFS_HPP_
#define CGNT_FILES_UNIXFS_UNIXFS_HPP_

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "proto/ProtoData.pb.h"

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
  UnixFS() = default;
  ~UnixFS() = default;

  UnixFS(const UnixFS& other) {
    data_type_ = other.data_type_;
    data_ = other.data_;
    filesize_ = other.filesize_;
    logger_ = common::logger::createLogger("unixfs");
  }

  /**
   *  @brief  Operator inequality.
   */
  bool operator!=(const UnixFS& rhs_unix_fs) {
    return (data_type_ != rhs_unix_fs.data_type_) &&
           (data_ != rhs_unix_fs.data_) && (filesize_ != rhs_unix_fs.filesize_);
  }

  /**
   *  @brief  Operator copy.
   */
  UnixFS& operator=(const UnixFS& rhs_unix_fs) {
    if (&rhs_unix_fs != this) {
      data_type_ = rhs_unix_fs.data_type_;
      data_ = rhs_unix_fs.data_;
      filesize_ = rhs_unix_fs.filesize_;
    }
    return *this;
  }

  /**
   *  @brief  Encode a data to protobuf.
   *  @param  data vector of bytes.
   *
   *  @return an unique ptr to protobuf class Data.
   */
  [[nodiscard]] std::unique_ptr<Data> EncodeMessage() const noexcept;
  /**
   *  @brief  Decode from probuf to data.
   *  @param  encoded protobuf class Data.
   *
   *  @return Status code.
   */
  Status DecodeMessage(const Data& encoded) noexcept;
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
                      const std::vector<uint8_t>& data) noexcept;
  /**
   *  @brief  Get file size.
   */
  [[nodiscard]] uint64_t GetFilesize() const noexcept { return filesize_; }
  /**
   *  @brief  Set file size.
   *  @param  filesize size of file to set.
   */
  void SetFilesize(uint64_t filesize) noexcept { filesize_ = filesize; }
  /**
   *  @brief  Get type of data.
   */
  [[nodiscard]] std::string GetDataType() const noexcept { return data_type_; }
  /**
   *  @brief  Get data.
   */
  [[nodiscard]] std::vector<uint8_t> GetData() const noexcept { return data_; }
  /**
   *  @brief  Set data.
   */
  void SetData(const std::vector<uint8_t>& data) noexcept { data_ = data; }

 private:
  /**
   *  @private Transform string type to enum int type of data message.
   *
   *  @param type type to convert.
   */
  [[nodiscard]] Data_DataType stringToDatatype(
      const std::string& type) const noexcept;

  std::string data_type_ = "file";
  std::vector<uint8_t> data_;
  uint64_t filesize_;

  common::logger::Logger logger_ = common::logger::createLogger("unixfs");
};

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio

#endif  // CGNT_FILES_UNIXFS_UNIXFS_HPP_
