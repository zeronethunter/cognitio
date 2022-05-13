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

/**
 *  @brief  This is special wrapper for filesystem datastorage.
 *
 *  Used to work with blocks of bytes.
 *
 *  Responsible for blocks management in the storage.
 *
 *  @attention For more information read documentation of Datastore interface.
 */
class Blockstorage {
 public:
  /**
   *  @brief  Construct from path.
   */
  explicit Blockstorage(const std::filesystem::path& path) noexcept {
    datastore::Filesystem<std::vector<uint8_t>> blocks(path);
    storage_ =
        std::make_unique<datastore::Filesystem<std::vector<uint8_t>>>(blocks);
  }
  /**
   *  @brief  Open Blockstorage.
   *
   *  @param path where to open.
   */
  Status Open(const std::filesystem::path& path);
  /**
   *  @brief  Open Blockstorage.
   */
  Status Open() { return storage_->Open(); }
  /**
   *  @brief  Get root of Blockstorage.
   */
  std::filesystem::path Root() { return storage_->Root(); }
  /**
   *  @brief  Close Blockstorage.
   */
  Status Close();
  /**
   *  @brief  Put Value by CID in storage.
   */
  Status Put(const common::Cid& key, const std::vector<uint8_t>& value);
  /**
   *  @brief  Get Value by CID from storage.
   */
  std::pair<Status, std::vector<uint8_t>> Get(const common::Cid& key);
  /**
   *  @brief  Delete Value by CID from storage.
   */
  Status Delete(const common::Cid& key);
  /**
   *  @brief  Check if key is in storage.
   */
  bool Has(const common::Cid& key) { return storage_->Has(key); }
  /**
   *  @brief  Put many Values by their CIDs in storage.
   */
  Status PutMany(
      const std::set<std::pair<common::Cid, std::vector<uint8_t>>>& source);
  /**
   *  @brief  Get many Value by their CIDs from storage.
   */
  std::pair<Status, std::set<std::vector<uint8_t>>> GetMany(
      const std::set<common::Cid>& source);
  /**
   *  @brief  Delete many Values by their CIDs in storage.
   */
  Status DeleteMany(const std::set<common::Cid>& source);

 private:
  std::unique_ptr<datastore::Filesystem<std::vector<uint8_t>>> storage_;
};

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_BLOCKSTORE_BLOCKSTORE_HPP_
