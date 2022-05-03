//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "files/unixfs/unixfs.hpp"

namespace cognitio {
namespace files {
namespace unixfs {

std::unique_ptr<Data> UnixFS::EncodeMessage(const std::vector<uint8_t>& data) {
  Data message;
  if (data_type_ == "raw") {
    message.set_type(Data_DataType_Raw);
  } else if (data_type_ == "directory") {
    message.set_type(Data_DataType_Directory);
  } else if (data_type_ == "file") {
    message.set_type(Data_DataType_File);
  } else if (data_type_ == "metadata") {
    message.set_type(Data_DataType_Metadata);
  } else if (data_type_ == "symlink") {
    message.set_type(Data_DataType_Symlink);
  }
  message.set_data(std::string(data.begin(), data.end()));
  message.set_filesize(filesize_);
  for (const uint64_t& blocksize : blocksizes_) {
    message.add_blocksizes(blocksize);
  }

  return std::make_unique<Data>(message);
}
std::vector<uint8_t> UnixFS::DecodeMessage(const Data& encoded) {
  return std::vector<uint8_t>(encoded.data().begin(), encoded.data().end());
}
Status UnixFS::CreateUnixFS(const std::string& datatype, uint64_t filesize,
                            const std::vector<uint64_t>& blocksizes) {
  if (datatype == "raw") {
    data_type_ = "raw";
  } else if (datatype == "directory") {
    data_type_ = "directory";
  } else if (datatype == "file") {
    data_type_ = "file";
  } else if (datatype == "metadata") {
    data_type_ = "metadata";
  } else if (datatype == "symlink") {
    data_type_ = "symlink";
  } else {
    return Status(StatusCode::CANCELLED,
                  "Type + " + datatype + " doesn't exist");
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
  return Status::OK;
}

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio
