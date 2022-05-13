//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "files/unixfs/unixfs.hpp"

namespace cognitio {
namespace files {
namespace unixfs {

Data_DataType UnixFS::stringToDatatype(const std::string& type) const noexcept {
  if (type == "raw") {
    return Data_DataType_Raw;
  } else if (type == "directory") {
    return Data_DataType_Directory;
  } else if (type == "file") {
    return Data_DataType_File;
  } else if (type == "metadata") {
    return Data_DataType_Metadata;
  } else if (type == "symlink") {
    return Data_DataType_Symlink;
  } else {
    logger_->error("Can't decode data message. Wrong type {}.", type);
  }
}

std::unique_ptr<Data> UnixFS::EncodeMessage() const noexcept {
  if (is_created_) {
    Data message;
    message.set_type(stringToDatatype(data_type_));
    if (!data_.empty()) {
      message.set_data(std::string(data_.begin(), data_.end()));
    }
    if (filesize_) {
      message.set_filesize(filesize_);
    }
    if (!blocksizes_.empty()) {
      for (const uint64_t& blocksize : blocksizes_) {
        message.add_blocksizes(blocksize);
      }
    }
    return std::make_unique<Data>(message);
  } else {
    logger_->error("Can't encode data message. Create it first.");
  }
}

Status UnixFS::DecodeMessage(const Data& encoded) noexcept {
  if (encoded.IsInitialized()) {
    if (Data_DataType_IsValid(encoded.type())) {
      data_type_ = Data_DataType_Name(encoded.type());
    } else {
      return Status(StatusCode::INVALID_ARGUMENT, "Data type is invalid.");
    }
    if (!encoded.blocksizes().empty()) {
      for (const uint64_t& blocksize : encoded.blocksizes()) {
        blocksizes_.push_back(blocksize);
      }
    }
    if (encoded.filesize()) {
      filesize_ = encoded.filesize();
    }
    if (!encoded.data().empty()) {
      data_ =
          std::vector<uint8_t>(encoded.data().cbegin(), encoded.data().cend());
    }
    is_created_ = true;
    return Status::OK;
  } else {
    logger_->error("Can't decode data message. Initialize it first.");
  }
}

Status UnixFS::CreateUnixFS(const std::string& datatype, uint64_t filesize,
                            const std::vector<uint64_t>& blocksizes,
                            const std::vector<uint8_t>& data) noexcept {
  int type = stringToDatatype(datatype);
  if (Data_DataType_IsValid(type)) {
    data_type_ = Data_DataType_Name(type);
  } else {
    return Status(StatusCode::INVALID_ARGUMENT,
                  "Data type " + datatype + " is invalid.");
  }
  if (filesize) {
    filesize_ = filesize;
  } else {
    return Status(StatusCode::INVALID_ARGUMENT, "Empty filesize");
  }
  if (!blocksizes.empty()) {
    blocksizes_ = blocksizes;
  } else {
    return Status(StatusCode::INVALID_ARGUMENT, "Empty blocksizes");
  }
  if (!data.empty()) {
    data_ = data;
  } else {
    return Status(StatusCode::INVALID_ARGUMENT, "Empty data");
  }
  is_created_ = true;
  return Status::OK;
}

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio
