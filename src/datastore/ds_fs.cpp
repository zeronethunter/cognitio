#include "datastore/ds_fs.hpp"

#include <fstream>

namespace cognitio {
namespace datastore {

template <typename Value>
Status Filesystem<Value>::Open(const std::filesystem::path& path) {
  path_ = path;
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

  file.write(static_cast<char*>(&value), sizeof(Value));

  file.close();

  return Status::OK;
}

template <typename Value>
std::pair<Status, Value> Filesystem<Value>::Get(const cid::Cid& key) {
  std::string filename = key.ToString();
  if (!std::filesystem::exists(path_ / filename)) {
    return std::pair<Status, Value>(Status(StatusCode::NOT_FOUND, " not found"), Value());
  }

  std::fstream file;
  file.open(filename, std::ios::binary);

  if (!file.is_open()) {
    return std::pair<Status, Value>(Status(StatusCode::CANCELLED, "Can not create " + filename), Value());
  }

  Value result;
  file.read(static_cast<char*>(&result), sizeof(Value));

  file.close();

  return std::pair<Status, Value>(Status::OK, result);
}

template <typename Value>
Status Filesystem<Value>::Delete(const cid::Cid& key) {
  std::string filename = key.ToString();
  if (std::filesystem::remove(path_ / filename)) {
    return Status::OK;
  }
  return Status(StatusCode::NOT_FOUND, " not found");
}

template <typename Value>
Status Filesystem<Value>::PutMany(
    const std::set<std::pair<cid::Cid, Value>>& source) {
  for (const auto& input : source) {
    Status err = Put(input.first, input.second);
    if (!err.ok()) {
      return err;
    }
  }
  return Status::OK;
}

template <typename Value>
std::pair<Status, std::set<Value>> Filesystem<Value>::GetMany(
    const std::set<cid::Cid>& source) {
  std::set<Value> result;
  for (const auto& key : source) {
    std::pair<Status, Value> it = Get(key);
    result.insert(it.second);
    if (!it.first.ok()) {
      return std::pair<Status, std::set<Value>>(it.first, std::set<Value>());
    }
  }
  return std::pair<Status, Value>(Status::OK, result);
}

template <typename Value>
Status Filesystem<Value>::DeleteMany(const std::set<cid::Cid>& source) {
  for (const auto& key : source) {
    Status err = Delete(key);
    if (!err.ok()) {
      return err;
    }
  }
  return Status::OK;
}

}  // namespace datastore
}  // namespace cognitio
