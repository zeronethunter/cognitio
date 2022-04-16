//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "files/unixfs/unixfs.hpp"

namespace cognitio {
namespace files {
namespace unixfs {

template <typename Options>
void UnixFS<Options>::marshal() {
}
template <typename Options>
std::unique_ptr<UnixFS<Options>*> UnixFS<Options>::unmarshaled(
    const std::string& marshaled) {
}
template <typename Options>
UnixFS<Options>::UnixFS(const Options& options) {
}
template <typename Options>
void UnixFS<Options>::AddBlockSize(size_t size_in_bytes) {
}
template <typename Options>
void UnixFS<Options>::RemoveBlockSize(size_t index) {
}
template <typename Options>
size_t UnixFS<Options>::Filesize() {
}
template <typename Options>
void UnixFS<Options>::SetMode(size_t mode) {
}

}  // namespace unixfs
}  // namespace files
}  // namespace cognitio
