//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/gc/gc.hpp"

namespace cognitio {
namespace repo {
namespace garbage {

template <typename Key, typename CID, typename Value, typename Options>
void Gc(const pinner::PinManager<Key, CID, Value, Options>& pins,
        const blockstorage::Blockstorage<Key, CID, Value, Options>& blockstorage,
        const datastore::Datastore<Key, Value, Options>& root) {
}
template <typename Key, typename CID, typename Value, typename Options>
std::set<CID> CreateMarkedSet(
    const pinner::PinManager<Key, CID, Value, Options>& pins,
    const blockstorage::Blockstorage<Key, CID, Value, Options>& blockstorage,
    const datastore::Datastore<Key, Value, Options>& root) {
}
template <typename Key, typename CID, typename Value, typename Options>
void RemoveBlock(
    const CID& cid,
    const blockstorage::Blockstorage<Key, CID, Value, Options>& blockstore) {
}
template <typename Key, typename CID, typename Value, typename Options>
void DeleteUnmarkedBlocks(
    blockstorage::Blockstorage<Key, CID, Value, Options> blockstore,
    std::set<CID> marked_set) {
}

}  // namespace gc
}  // namespace repo
}  // namespace cognitio