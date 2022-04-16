//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "unixfs.pb.h"

#ifndef CGNT_FILES_UNIXFS_PROTO_MANAGER_PROTO_MANAGER_HPP_
#define CGNT_FILES_UNIXFS_PROTO_MANAGER_PROTO_MANAGER_HPP_

namespace cognitio {
namespace files {
namespace unixfs {
namespace protomanager {

template <typename Options>
class UnixFSData {
  //! Block of data that we will use as a message

 public:
  UnixFSData(
      const Options& options);  //! Set every field in data, unix_time and
                                //! metadata to create UnixFS Data value

 private:
  Data data_;
  UnixTime u_time;
  Metadata metadata;
};

}  // namespace protomanager
}  // namespace unixfs
}  // namespace files
}  // namespace cognitio

#endif  // CGNT_FILES_UNIXFS_PROTO_MANAGER_PROTO_MANAGER_HPP_
