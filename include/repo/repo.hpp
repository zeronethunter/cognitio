// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_REPO_HPP_
#define CGNT_REPO_REPO_HPP_

#include <filesystem>

#include "datastore/ds_fs.hpp"
#include "pinner/pin-manager.hpp"
#include "repo/gc/gc.hpp"

namespace cognitio {
namespace repo {
//! Repository part of project.
//! Responsible for creating, managing and deleting the repository.
//! Creates Garbage Collector, Config File and Storages.
//! And also manages Pins.

template <typename Key, typename CID, typename Value, typename Options>
class Repo {
 public:
  explicit Repo(const std::filesystem::path& path,
                const datastore::Filesystem<Key, Value, Options>& datastore);
  void
  Init();  //! Initializing the repository, namely starting gc and creating pins
  void Open(); //! Open the repository
  void Close(); //! Close the repository, clean it up

 private:
  bool closed_ = true;
  std::filesystem::path path_;
  datastore::Filesystem<Key, Value, Options> root_;
  datastore::Filesystem<Key, Value, Options> datastore_;
  datastore::Filesystem<Key, Value, Options> key_;
  pinner::PinManager<Key, CID, Value, Options> pins_;
  blockstorage::Blockstorage<Key, CID, Value, Options> pinned_block_storage_;
  void OpenRoot();
  void OpenLock();
  void CloseLock();
};

}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_REPO_HPP_
