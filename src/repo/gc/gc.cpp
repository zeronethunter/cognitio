#include "repo/gc/gc.hpp"

namespace cognitio {
namespace repo {
namespace garbage {

void GcInit(const pinner::PinManager& pins,
            const blockstorage::Blockstorage& blockstorage,
            const datastore::Filesystem<std::string>& root) noexcept {}

}  // namespace garbage
}  // namespace repo
}  // namespace cognitio