//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/block_storage/block_storage.hpp"

namespace cognitio::repo::blockstorage {

Status blockstorage::Blockstorage::Open(const std::filesystem::path& path) {
  return storage_->Open(path);
}

Status blockstorage::Blockstorage::Close() { return storage_->Close(); }

Status blockstorage::Blockstorage::Put(const common::Cid& key,
                                       const std::vector<uint8_t>& value) {
  return storage_->Put(key, value);
}

std::pair<Status, std::vector<uint8_t>> blockstorage::Blockstorage::Get(
    const common::Cid& key) {
  return storage_->Get(key);
}

Status blockstorage::Blockstorage::Delete(const common::Cid& key) {
  return storage_->Delete(key);
}

Status blockstorage::Blockstorage::PutMany(
    const std::set<std::pair<common::Cid, std::vector<uint8_t>>>& source) {
  return storage_->PutMany(source);
}

std::pair<Status, std::set<std::vector<uint8_t>>>
blockstorage::Blockstorage::GetMany(const std::set<common::Cid>& source) {
  return storage_->GetMany(source);
}

Status blockstorage::Blockstorage::DeleteMany(
    const std::set<common::Cid>& source) {
  return storage_->DeleteMany(source);
}

}  // namespace cognitio
