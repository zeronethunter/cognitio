//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/block_storage/block_storage.hpp"

namespace cognitio {
namespace repo {
namespace blockstorage {

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::Open() {}

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::Close() {}

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::Put(
    const CID& cid, const Value& value, const Options& options) {
  return storage_.Put(cid, value, options);
}

template <typename Key, typename CID, typename Value, typename Options>
Value Blockstorage<Key, CID, Value, Options>::Get(const CID& cid,
                                                  const Options& options) {
  return storage_.Get(cid, options);
}

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::Delete(
    const CID& cid, const Options& options) {
  return storage_.Delete(cid, options);
}

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::PutMany(
    const std::set<std::pair<CID, Value>>& source, const Options& options) {
  return storage_.PutMany(source, options);
}

template <typename Key, typename CID, typename Value, typename Options>
std::set<Value> Blockstorage<Key, CID, Value, Options>::GetMany(
    const std::set<CID>& source, const Options& options) {
  return storage_.GetMany(source, options);
}

template <typename Key, typename CID, typename Value, typename Options>
datastore::DsError Blockstorage<Key, CID, Value, Options>::DeleteMany(
    const std::set<CID>& source, const Options& options) {
  return storage_.DeleteMany(source, options);
}

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio
