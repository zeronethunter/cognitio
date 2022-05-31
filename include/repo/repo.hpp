// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_REPO_HPP_
#define CGNT_REPO_REPO_HPP_

#include <chrono>
#include <condition_variable>
#include <exception>
#include <filesystem>
#include <memory>
#include <thread>

#include "block_storage/block_storage.hpp"
#include "common/logger/logger.hpp"
#include "common/utils/repo.hpp"
#include "config/config.hpp"
#include "linked_data/proto_block.hpp"
#include "repo/pinner/pin-manager.hpp"

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
  typedef linked_data::ProtoBlock ProtoBlock;

  /**
   *  @brief Constructor of repository from path.
   *
   *  @param path path to new repository.
   */
  explicit Repo(const std::filesystem::path &path =
                    std::filesystem::path(common::utils::GetDefaultRepoPath()))

      noexcept;

  /**
   *  @brief Constructor of repository from name.
   *
   *  @param name name of new repository.
   */
  explicit Repo(const std::string &name = ".cognitio")

      noexcept;

  /**
   *  @brief Initializing the repository, namely starting gc and creating pins.
   */
  Status Init()

      noexcept;

  /**
   *  @brief Close the repository, clean it up.
   */
  Status Close()

      noexcept {
    closed_ = true;

    return Status::OK;
  }

  /**
   *  @brief Add value by key in repo.
   *
   *  @param cid the key that will be used to put value.
   *  @param data block of bytes.
   */
  Status Add(const ProtoBlock &block, bool is_pinned = true)

      noexcept;

  /**
   *  @brief Delete value by key in repo.
   *
   *  @param cid the key that will be used to delete value.
   */
  Status Delete(const common::Cid &cid)

      noexcept {
    pinner_->UnPin(cid);
    return deleteByKey(cid);
  }

  /**
   *  @brief Get value by key in repo.
   *
   *  @param cid the key that will be used to delete value.
   *
   *  @return vector of bytes.
   */
  [[nodiscard]] ProtoBlock Get(const common::Cid &cid) const

      noexcept;

  /**
   *  @brief Check if key is in repo.
   *
   *  @param cid the key to find in storage.
   */
  [[nodiscard]] bool Has(const common::Cid &cid) const

      noexcept;

  /**
   *  @brief Check if repo is already exists.
   */
  bool Exists()

      noexcept;

  /**
   *  @return Path of root storage.
   */
  [[nodiscard]] std::filesystem::path Root() const

      noexcept {
    return root_->Root();
  }

  void RunGc() {
    is_running_ = true;
    gc_ = std::thread(&repo::Repo<StoreValue>::startGarbageCollector, this);
    logger_->debug("Gc started.");
  }

  void Shutdown() {
    is_running_ = false;
    cv_.notify_all();

    if (gc_.joinable()) {
      logger_->debug("Gc shutdown.");
      gc_.join();
    }
  }

  config::Config &GetConfig() noexcept { return config_; }
  std::shared_ptr<config::Config> GetForkedConfig() noexcept {
    return config_.getForkedInstance();
  }

  Status Reset();

 private:
  Status openRepo()

      noexcept;

  [[nodiscard]] std::string shard(const cognitio::common::Cid &cid,
                                  size_t name_length = 2) const noexcept;

  void initRepoStorage(const std::filesystem::path &path) noexcept;

  Status addByKey(const common::Cid &cid,
                  const std::vector<uint8_t> &data) noexcept;

  Status deleteByKey(const common::Cid &cid) noexcept;

  [[nodiscard]] std::vector<uint8_t> getByKey(
      const common::Cid &cid) const noexcept;

  size_t getDirSize(const std::filesystem::path &dir_path) const noexcept;

  /**
   *  @brief  Delete Unpinned blocks in block_storage.
   *
   *  @return status.
   */
  Status deleteUnmarkedBlock() noexcept;

  /**
   *  @brief  Garbage collector starting.
   */
  void startGarbageCollector() noexcept;

  bool closed_ = true;

  config::Config &config_;
  std::unique_ptr<datastore::Filesystem<StoreValue>> root_;
  std::unique_ptr<blockstorage::Blockstorage> blocks_;
  std::unique_ptr<pinner::PinManager> pinner_;
  std::thread gc_;
  std::atomic_bool is_running_;
  std::mutex mutex_;
  std::condition_variable cv_;

  common::logger::Logger logger_;
};

}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_REPO_HPP_
