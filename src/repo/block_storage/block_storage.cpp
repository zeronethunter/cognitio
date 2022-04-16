//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/block_storage/block_storage.hpp"

namespace cognitio {
namespace repo {
namespace blockstorage {

void Open();
void Close();
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::Put(const CID& cid,
                                                 const Value& value,
                                                 const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::Get(const CID& cid,
                                                 const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::Delete(const CID& cid,
                                                    const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::PutMany(
    const std::set<std::pair<CID, Value>>& source, const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::GetMany(
    const std::set<CID>& source, const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Blockstorage<Key, CID, Value, Options>::DeleteMany(
    const std::set<CID>& source, const Options& options) {
}

}  // namespace blockstorage
}  // namespace repo
}  // namespace cognitio