// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_REPO_HPP_
#define CGNT_REPO_REPO_HPP_

#include <filesystem>

#include "common/logger/logger.hpp"
#include "common/utils/repo.hpp"
#include "datastore/ds_fs.hpp"
#include "pinner/pin-manager.hpp"
#include "repo/gc/gc.hpp"

namespace cognitio {
namespace repo {

/**
 *  @brief  Repository part of project. Responsible for creating, managing and
 * deleting the repository.
 *
 *  Creates Garbage Collector, Config File and Storages.
 *
 *  And also manages Pins.
 *
 *  @tparam Buffer type of value to store in repo files. Default is vector of
 * bytes.
 */
template <typename StoreValue = std::string>
class Repo {
 public:
  /**
   *  @brief Constructor of repository from Filesystem.
   *
   *  @param root filesystem of repository.
   */
  explicit Repo(
      std::unique_ptr<datastore::Filesystem<StoreValue>> const& root) noexcept;
  /**
   *  @brief Constructor of repository from path.
   *
   *  @param path path to new repository.
   */
  explicit Repo(const std::filesystem::path& path =
                    std::filesystem::current_path() /
                    common::utils::GetDefaultRepoPath()) noexcept;
  /**
   *  @brief Constructor of repository from name.
   *
   *  @param name name of new repository.
   */
  explicit Repo(
      const std::string& name = common::utils::GetDefaultRepoPath()) noexcept;
  /**
   *  @brief Initializing the repository, namely starting gc and creating pins.
   */
  Status Init() noexcept;
  /**
   *  @brief Close the repository, clean it up.
   */
  Status Close() noexcept;
  /**
   *  @brief Add value by key in repo.
   *
   *  @param cid the key that will be used to put value.
   *  @param data block of bytes.
   */
  Status Add(const common::Cid& cid, const std::vector<uint8_t>& data) noexcept;
  /**
   *  @brief Delete value by key in repo.
   *
   *  @param cid the key that will be used to delete value.
   */
  Status Delete(const common::Cid& cid) noexcept;
  /**
   *  @brief Check if repo is already exists.
   */
  bool Exists() noexcept;

 private:
  Status openRepo() noexcept;
  std::string shard(const cognitio::common::Cid& cid, size_t name_length = 2);
  void initRepoStorage(const std::filesystem::path& path);

  bool closed_ = true;
  std::unique_ptr<datastore::Filesystem<StoreValue>> root_;
  std::unique_ptr<blockstorage::Blockstorage> blocks_;
  common::logger::Logger logger_ = common::logger::createLogger("Repo logger");
  //  pinner::PinManager<Key, Value, Options> pins_;
  //  blockstorage::Blockstorage<Key, Value, Options> pinned_block_storage_;

  //  Status OpenLock();
  //  Status CloseLock();
};

}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_REPO_HPP_
