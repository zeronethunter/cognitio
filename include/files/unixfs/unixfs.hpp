// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "proto_manager/proto-manager.hpp"
#include "proto_manager/unixfs.pb.h"

#ifndef CGNT_FILES_UNIXFS_UNIXFS_HPP_
#define CGNT_FILES_UNIXFS_UNIXFS_HPP_

namespace cognitio {
namespace files {
namespace unixfs {

template <typename Options>
class UnixFS {
  //! Managing data blocks:
  //! changing, managing, encrypting, decrypting in Protobuf

 public:
  void marshal();  //! ... (encode to probuf bytes array)
  std::unique_ptr<UnixFS*> unmarshaled(
      const std::string& marshaled);  //! ...(decode from probuf)
  UnixFS(const Options& options = {
    type : 'file'
  });  //! Initialize protobuf class Data with Options
  void AddBlockSize(size_t size_in_bytes);  //! Add block size...
  void RemoveBlockSize(size_t index);       //! Remove block size by index
  size_t Filesize();          //! Return 0 or data.length + sum(blocksizes)
  void SetMode(size_t mode);  //! Set mode ('r' or 'w')
 private:
  protomanager::UnixFSData<Options> data_;
};

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio

#endif  // CGNT_FILES_UNIXFS_UNIXFS_HPP_
