// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CNGT_REPO_PINNER_PIN_MANAGER_HPP_
#define CNGT_REPO_PINNER_PIN_MANAGER_HPP_

#include <set>

#include "datastore/ds_fs.hpp"
#include "repo/block_storage/block_storage.hpp"

namespace cognitio {
namespace repo {
namespace pinner {
/**
 *  @brief  Pin Manager.
 *
 *  Pins and Unpins blocks, so the Garbage Collector doesn't remove them
 */
class PinManager {
 public:
  PinManager() = default;
  PinManager(const blockstorage::Blockstorage& pinstore)
      : pinstore_(pinstore) {}

  PinManager(const PinManager& pin_manager) = delete;
  PinManager& operator=(const PinManager& pin_manager) = delete;

  /**
   *  @brief  Put key to pinstore directly.
   *
   *  @param cid key to pin.
   */
  void PinDirectly(const common::Cid& cid);
  /**
   *  @brief  Put key to pinstore recursively.
   *
   *  @param cid key to pin recursively.
   */
  void PinRecursively(const common::Cid& cid);
  /**
   *  @brief  Delete key in pinstore.
   *
   *  @param cid key to unpin.
   */
  void UnPin(const common::Cid& cid);

 private:
  blockstorage::Blockstorage pinstore_;
  std::set<common::Cid> pins_;
};

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio
#endif  // CNGT_REPO_PINNER_PIN_MANAGER_HPP_
