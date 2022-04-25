// Copyright (c)  override 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_DATASTORE_DS_FS_HPP_
#define CGNT_DATASTORE_DS_FS_HPP_

#include <filesystem>

#include "common/status.hpp"
#include "datastore/datastore.hpp"
#include "common/multiformats/cid.hpp"

namespace cognitio {
namespace datastore {
//! This is implementation of datastore filesystem
//! Responsible for file management in the storage

template <typename Value>
class Filesystem : public Datastore<cid::Cid, Value, Status> {
 public:
  Status Open() override;
  Status Close() override;
  Status Put(const cid::Cid& key, const Value& value) override;
  Value Get(const cid::Cid& key) override;
  Status Delete(const cid::Cid& key) override;
  Status PutMany(const std::set<std::pair<cid::Cid, Value>>& source) override;
  std::set<Value> GetMany(const std::set<cid::Cid>& source) override;
  Status DeleteMany(const std::set<cid::Cid>& source) override;
 private:
  std::filesystem::path path_;
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DS_FS_HPP_
