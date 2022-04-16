//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/repo.hpp"

namespace cognitio {
namespace repo {

template <typename Key, typename CID, typename Value, typename Options>
Repo<Key, CID, Value, Options>::Repo(
    const std::filesystem::path& path,
    const datastore::Filesystem<Key, Value, Options>& datastore) {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::Init() {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::Open() {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::Close() {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::OpenRoot() {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::OpenLock() {
}
template <typename Key, typename CID, typename Value, typename Options>
void Repo<Key, CID, Value, Options>::CloseLock() {
}

}  // namespace repo
}  // namespace cognitio