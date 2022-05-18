// Copyright (c)  override 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_DATASTORE_DS_FS_HPP_
#define CGNT_DATASTORE_DS_FS_HPP_

#include <filesystem>
#include <utility>
#include <set>

#include "multiformats/cid.hpp"
#include "common/status.hpp"
#include "datastore/datastore.hpp"

namespace cognitio {
namespace datastore {

/**
 *  @brief  This is implementation of datastore filesystem with key of type Cid.
 *
 *  Responsible for file management in the storage.
 *
 *  @tparam Value type of value to put in local storage.
 *  @extends from Datastore interface.
 */
template <typename Value>
class Filesystem : Datastore<common::Cid, Value, Status> {
 public:
  explicit Filesystem(std::filesystem::path path) noexcept
      : path_(std::move(path)) {}

  Filesystem() = default;
  Filesystem(const Filesystem<Value>& filesystem) noexcept
      : path_(filesystem.path_) {}
  Filesystem& operator=(const Filesystem<Value>& filesystem) noexcept {
    path_ = filesystem.path_;
    return *this;
  }

  Status Open(const std::filesystem::path& path) noexcept override;

  Status Open() noexcept {
    if (std::filesystem::exists(path_)) {
      return Open(path_);
    }
    return {StatusCode::OK, path_.string() + "  is already opened."};
  }

  Status Close() noexcept override;

  [[nodiscard]] std::filesystem::path Root() const noexcept { return path_; }

  Status MakeDir(const std::string& dir_name) noexcept {
    if (!std::filesystem::exists(path_ / dir_name)) {
      if (std::filesystem::create_directory(path_ / dir_name)) {
        return Status::OK;
      }
      return {StatusCode::OK, "Can't create directory " + dir_name + "."};
    }
    return Status::OK;
  }

  Status Put(const common::Cid& key, const Value& value) noexcept override;

  std::pair<Status, Value> Get(const common::Cid& key) const noexcept override;

  Status Delete(const common::Cid& key) noexcept override;

  [[nodiscard]] bool Has(const common::Cid& key) const noexcept override {
    return std::filesystem::exists(key.ToString());
  }

  Status PutMany(
      const std::set<std::pair<common::Cid, Value>>& source) noexcept override;

  std::pair<Status, std::set<Value>> GetMany(
      const std::set<common::Cid>& source) const noexcept override;

  Status DeleteMany(const std::set<common::Cid>& source) noexcept override;

 private:
  std::filesystem::path path_;
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DS_FS_HPP_
