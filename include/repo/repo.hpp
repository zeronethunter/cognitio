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
//! \brief Repository part of project.
//! \brief Responsible for creating, managing and deleting the repository.
//! \brief Creates Garbage Collector, Config File and Storages.
//! \brief And also manages Pins.

class Repo {
 public:
  static Repo CreateRepo(
      const std::filesystem::path& path);  //! Create repository
  explicit Repo(datastore::Filesystem<std::vector<uint8_t>> const& root);
  Status
  Init();  //! Initializing the repository, namely starting gc and creating pins
  Status Open();   //! Open the repository
  Status Close();  //! Close the repository, clean it up

 private:
  bool closed_ = true;
  datastore::Filesystem<std::vector<uint8_t>> root_;
  blockstorage::Blockstorage blocks_;
  //  pinner::PinManager<Key, Value, Options> pins_;
  //  blockstorage::Blockstorage<Key, Value, Options> pinned_block_storage_;
  Status OpenRoot();
  //  Status OpenLock();
  //  Status CloseLock();
};

}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_REPO_HPP_
