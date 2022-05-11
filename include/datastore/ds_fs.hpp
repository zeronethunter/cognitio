// Copyright (c)  override 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_DATASTORE_DS_FS_HPP_
#define CGNT_DATASTORE_DS_FS_HPP_

#include <filesystem>

#include "common/multiformats/cid.hpp"
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
 *  @extends from Datastore virtual class
 */
template <typename Value>
class Filesystem :  Datastore<common::Cid, Value, Status> {
 public:
  Status Open(const std::filesystem::path& path) override;

  Status Close() override;

  std::filesystem::path Root() { return path_; }

  Status Put(const common::Cid& key, const Value& value) override;

  std::pair<Status, Value> Get(const common::Cid& key) override;

  Status Delete(const common::Cid& key) override;

  Status PutMany(const std::set<std::pair<common::Cid, Value>>& source) override;

  std::pair<Status, std::set<Value>> GetMany(
      const std::set<common::Cid>& source) override;

  Status DeleteMany(const std::set<common::Cid>& source) override;

 private:
  std::filesystem::path path_;
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DS_FS_HPP_
