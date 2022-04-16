//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <set>

#include "datastore/ds_fs.hpp"

#ifndef CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
#define CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_

namespace cognitio {
namespace repo {
namespace blockstorage {

//! This is special wrapper for datastore
//! Used to work with blocks localy (CIDs)
template <typename Key, typename CID, typename Value, typename Options>
class Blockstorage : public datastore::Filesystem<Key, Value, Options> {
 public:
  void Open();   //! Open Blockstorage (Filesystem for managing blocks)
  void Close();  //! Close Blockstorage
  void Put(const CID& cid, const Value& value,
           const Options& options);  //! Put Value by CID in storage
  void Get(const CID& cid,
           const Options& options);  //! Get Value by CID from storage
  void Delete(const CID& cid,
              const Options& options);  //! Delete Value by CID from storage
  void PutMany(const std::set<std::pair<CID, Value>>& source,
               const Options& options =
                   Options());  //! Put many Values by their CIDs in storage
  void GetMany(const std::set<CID>& source,
               const Options& options =
                   Options());  //! Get many Value by their CIDs from storage
  void DeleteMany(
      const std::set<CID>& source,
      const Options& options =
          Options());  //! Delete many Values by their CIDs in storage
};

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
