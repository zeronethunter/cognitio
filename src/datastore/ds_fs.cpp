#include "datastore/ds_fs.hpp"

namespace cognitio {
namespace datastore {

void Open() {
}
void Close() {
}
template <typename Key, typename Value, typename Options>
void Put(const Key& key, const Value& value, const Options& options) {
}
template <typename Key, typename Options>
void Get(const Key& key, const Options& options) {
}
template <typename Key, typename Options>
void Delete(const Key& key, const Options& options) {
}
template <typename Key, typename Value, typename Options>
void PutMany(const std::set<std::pair<Key, Value>>& source,
             const Options& options = Options()) {
}
template <typename Key, typename Options>
void GetMany(const std::set<Key>& source, const Options& options = Options()) {
}
template <typename Key, typename Options>
void DeleteMany(const std::set<Key>& source,
                const Options& options = Options()) {
}

}  // namespace datastore
}  // namespace cognitio
