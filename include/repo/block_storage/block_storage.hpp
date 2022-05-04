//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
#define CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_

#include <set>

#include "common/block/block.hpp"
#include "common/multiformats/cid.hpp"
#include "common/status.hpp"
#include "datastore/ds_fs.hpp"

namespace cognitio {
namespace repo {
namespace blockstorage {

//! This is special wrapper for datastore
//! Used to work with blocks localy (CIDs)
class Blockstorage {
 public:
  Status Open(const std::filesystem::path& path);  //! Open Blockstorage
  std::filesystem::path Root() { return storage_.Root(); }
  Status Close();  //! Close Blockstorage
  Status Put(
      const cid::Cid& key,
      const std::vector<uint8_t>& value);  //! Put Value by CID in storage
  std::pair<Status, std::vector<uint8_t>> Get(
      const cid::Cid& key);            //! Get Value by CID from storage
  Status Delete(const cid::Cid& key);  //! Delete Value by CID from storage
  Status PutMany(const std::set<std::pair<cid::Cid, std::vector<uint8_t>>>&
                     source);  //! Put many Values by their CIDs in storage
  std::pair<Status, std::set<std::vector<uint8_t>>> GetMany(
      const std::set<cid::Cid>&
          source);  //! Get many Value by their CIDs from storage
  Status DeleteMany(
      const std::set<cid::Cid>& source);  //! Delete many Values by
                                          //! their CIDs in storage
 private:
  datastore::Filesystem<std::vector<uint8_t>> storage_;  //! FS datastorage
};

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
