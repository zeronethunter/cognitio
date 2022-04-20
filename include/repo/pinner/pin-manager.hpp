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
//! Pin Manager
//! Pins and Unpins blocks, so the Garbage Collector doesn't remove them

template <typename Key, typename CID, typename Value, typename Options>
class PinManager {
 public:
  PinManager() = default;
  PinManager(const datastore::Datastore<Key, Value, Options>& pinstore) {
    pinstore_ = pinstore;
  }
  void PinDirectly(
      const CID& cid,
      const Options& options = Options());  //! Put key to pinstore directly
  void UnPin(const CID& cid,
             const Options& options);  //! Delete key in pinstore
  void PinRecursively(
      const CID& cid,
      const Options& options = Options());  //! Put key to pinstore recursively
  void DirectKeys(const Options& options);
  void RecursiveKeys(const Options& options);
  void IndirectKeys(const Options& options);

 private:
  datastore::Datastore<Key, Value, Options> pinstore_;
  blockstorage::Blockstorage<Key, CID, Value, Options> blockstorage_;
  std::set<CID> direct_pins_;
  std::set<CID> recursive_pins_;
};

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio
#endif  // CNGT_REPO_PINNER_PIN_MANAGER_HPP_
