#include "datastore/ds_fs.hpp"

#include <fstream>

namespace cognitio {
namespace datastore {

template <typename Value>
Status Filesystem<Value>::Open() {
  if (std::filesystem::create_directory(path_)) {
    return Status::OK;
  }
  return Status(StatusCode::OK, path_.string() + " was created");
}

template <typename Value>
Status Filesystem<Value>::Close() {}

template <typename Value>
Status Filesystem<Value>::Put(const cid::Cid& key, const Value& value) {
  std::string filename = key.ToString();
  if (std::filesystem::exists(path_ / filename)) {
    return Status(StatusCode::ALREADY_EXISTS, " already exists");
  }
  std::fstream file;
  file.open(filename, std::ios::binary);

  if (!file.is_open()) {
    return Status(StatusCode::CANCELLED, "Can not create " + filename);
  }

  file.write(static_cast<char *>(&value), sizeof(Value));

  file.close();

  return Status::OK;
}

template <typename Value>
Value Filesystem<Value>::Get(const cid::Cid& key) {
  std::string filename = key.ToString();
  if (!std::filesystem::exists(path_ / filename)) {
    return Status(StatusCode::NOT_FOUND, " not found");
  }

  std::fstream file;
  file.open(filename, std::ios::binary);

  if (!file.is_open()) {
    return Status(StatusCode::CANCELLED, "Can not create " + filename);
  }

  Value result;
  file.read(static_cast<char *>(&result), sizeof(Value));

  file.close();

  return result;
}

template <typename Value>
Status Filesystem<Value>::Delete(const cid::Cid& key) {}

template <typename Value>
Status Filesystem<Value>::PutMany(
    const std::set<std::pair<cid::Cid, Value>>& source) {}

template <typename Value>
std::set<Value> Filesystem<Value>::GetMany(const std::set<cid::Cid>& source) {}

template <typename Value>
Status Filesystem<Value>::DeleteMany(const std::set<cid::Cid>& source) {}

}  // namespace datastore
}  // namespace cognitio
