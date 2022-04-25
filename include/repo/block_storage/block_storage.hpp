//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
#define CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_

#include <set>

#include "common/status.hpp"
#include "datastore/ds_fs.hpp"

namespace cognitio {
namespace repo {
namespace blockstorage {

//! This is special wrapper for datastore
//! Used to work with blocks localy (CIDs)
template <typename Key, typename Value>
class Blockstorage {
 public:
  Status Open();   //! Open Blockstorage
  Status Close();  //! Close Blockstorage
  Status Put(const Key& key,
             const Value& value);  //! Put Value by CID in storage
  Value Get(const Key& key);       //! Get Value by CID from storage
  Status Delete(const Key& key);   //! Delete Value by CID from storage
  Status PutMany(const std::set<std::pair<Key, Value>>&
                     source);  //! Put many Values by their CIDs in storage
  std::set<Value> GetMany(
      const std::set<Key>&
          source);  //! Get many Value by their CIDs from storage
  Status DeleteMany(const std::set<Key>& source);  //! Delete many Values by
                                                   //! their CIDs in storage
 private:
  datastore::Filesystem<Key, Value> storage_;  //! FS datastorage
};

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
