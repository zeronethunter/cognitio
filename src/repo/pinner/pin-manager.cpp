//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/pinner/pin-manager.hpp"

namespace cognitio {
namespace repo {
namespace pinner {

template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::PinDirectly(const CID& cid,
                                                       const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::UnPin(const CID& cid,
                                                 const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::PinRecursively(
    const CID& cid, const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::DirectKeys(const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::RecursiveKeys(
    const Options& options) {
}
template <typename Key, typename CID, typename Value, typename Options>
void PinManager<Key, CID, Value, Options>::IndirectKeys(
    const Options& options) {
}

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio
