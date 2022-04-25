//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/block_storage/block_storage.hpp"

namespace cognitio {
namespace repo {
namespace blockstorage {

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::Open() {
  if ()
}

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::Close() {}

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::Put(
    const Key& key, const Value& value, const Options& options) {
  return storage_.Put(key, value, options);
}

template <typename Key, typename Value, typename Options>
Value Blockstorage<Key, Value, Options>::Get(const Key& key,
                                                  const Options& options) {
  return storage_.Get(key, options);
}

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::Delete(
    const Key& key, const Options& options) {
  return storage_.Delete(key, options);
}

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::PutMany(
    const std::set<std::pair<Key, Value>>& source, const Options& options) {
  return storage_.PutMany(source, options);
}

template <typename Key, typename Value, typename Options>
std::set<Value> Blockstorage<Key, Value, Options>::GetMany(
    const std::set<Key>& source, const Options& options) {
  return storage_.GetMany(source, options);
}

template <typename Key, typename Value, typename Options>
Status Blockstorage<Key, Value, Options>::DeleteMany(
    const std::set<Key>& source, const Options& options) {
  return storage_.DeleteMany(source, options);
}

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio
