//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_REPO_GC_GC_HPP_
#define CGNT_REPO_GC_GC_HPP_

#include <set>
#include <thread>

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
void GcInit(const pinner::PinManager& pins,
            const blockstorage::Blockstorage& blockstorage,
            const datastore::Filesystem<std::string>& root) noexcept;

/**
 *  @brief  Pins set of blocks in datastore.
 *
 *  @param pins pin manager that will give GC what to delete.
 *  @param blockstorage storage where GC should delete unpinned blocks.
 *  @param root root path of repo, used to manage lock file.
 */
std::set<common::Cid> CreateMarkedSet(
    const pinner::PinManager& pins,
    const blockstorage::Blockstorage& blockstorage,
    const datastore::Filesystem<std::string>& root) noexcept;

/**
 *  @brief  Remove block in block_storage.
 *
 *  @param cid to delete.
 *  @param blockstorage where to delete.
 */
void RemoveBlock(const common::Cid& cid,
                 const blockstorage::Blockstorage& blockstorage) noexcept;

/**
 *  @brief  Delete Unpinned blocks in block_storage.
 *
 *  @param marked_set set of block CIDs to delete.
 *  @param blockstorage where to delete.
 */
void DeleteUnmarkedBlocks(const blockstorage::Blockstorage& blockstorage,
                          const std::set<common::Cid>& marked_set) noexcept;

}  // namespace garbage
}  // namespace repo
}  // namespace cognitio

#endif  // CGNT_REPO_GC_GC_HPP_
