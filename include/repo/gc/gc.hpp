//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_GC_GC_HPP_
#define CGNT_REPO_GC_GC_HPP_

#include <coroutine>
#include <set>

#include "repo/pinner/pin-manager.hpp"

namespace cognitio {
namespace repo {
namespace garbage {

/**
 *  @brief  Garbage collector initialization.
 *
 *  @param pins pin manager that will give GC what to delete.
 *  @param blockstorage storage where GC should delete unpinned blocks.
 *  @param root root path of repo, used to manage lock file.
 */
std::coroutine_handle<void> GcInit(
    const pinner::PinManager& pins,
    const blockstorage::Blockstorage& blockstorage,
    const datastore::Filesystem<std::string>& root);

template <typename Key, typename CID, typename Value, typename Options>
std::set<CID> CreateMarkedSet(
    const pinner::PinManager<Key, CID, Value, Options>& pins,
    const blockstorage::Blockstorage<Key, CID, Value, Options>& blockstorage,
    const datastore::Datastore<Key, Value, Options>&
        root);  //! Pins set of blocks in datastore

template <typename Key, typename CID, typename Value, typename Options>
void RemoveBlock(const CID& cid,
                 const blockstorage::Blockstorage<Key, CID, Value, Options>&
                     blockstorage);  //! Remove block in block_storage

template <typename Key, typename CID, typename Value, typename Options>
void DeleteUnmarkedBlocks(
    blockstorage::Blockstorage<Key, CID, Value, Options> blockstorage,
    std::set<CID> marked_set);  //! Delete Unpinned blocks in block_storage

}  // namespace garbage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_GC_GC_HPP_
