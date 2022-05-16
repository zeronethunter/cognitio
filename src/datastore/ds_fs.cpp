#include "datastore/ds_fs.hpp"

#include <fstream>

namespace cognitio {
namespace datastore {

template <typename Value>
Status Filesystem<Value>::Open(const std::filesystem::path& path) noexcept {
  path_ = path;
  if (!std::filesystem::exists(path)) {
    if (std::filesystem::create_directory(path_)) {
      return Status::OK;
    }
    return {StatusCode::FAILED, "Can't open storage."};
  }
}

template <typename Value>
Status Filesystem<Value>::Close() noexcept {}

template <typename Value>
Status Filesystem<Value>::Put(const common::Cid& key,
                              const Value& value) noexcept {
  std::string filename = key.ToString();
  if (std::filesystem::exists(path_ / filename)) {
    return {StatusCode::ALREADY_EXISTS, " already exists"};
  }
  std::fstream file;
  file.open(filename, std::ios::binary);

  if (!file.is_open()) {
    return {StatusCode::CANCELLED, "Can not create " + filename};
  }

  file.write(static_cast<char*>(&value), sizeof(Value));

  file.close();

  return Status::OK;
}

template <typename Value>
std::pair<Status, Value> Filesystem<Value>::Get(
    const common::Cid& key) const noexcept {
  std::string filename = key.ToString();
  if (!std::filesystem::exists(path_ / filename)) {
    return std::pair<Status, Value>({StatusCode::NOT_FOUND, " not found"},
                                    Value());
  }

  std::fstream file;
  file.open(filename, std::ios::binary);

  if (!file.is_open()) {
    return std::pair<Status, Value>(
        {StatusCode::CANCELLED, "Can not create " + filename}, Value());
  }

  Value result;
  file.read(static_cast<char*>(&result), sizeof(Value));

  file.close();

  return std::pair<Status, Value>(Status::OK, result);
}

template <typename Value>
Status Filesystem<Value>::Delete(const common::Cid& key) noexcept {
  std::string filename = key.ToString();
  if (std::filesystem::remove(path_ / filename)) {
    return Status::OK;
  }
  return {StatusCode::NOT_FOUND, filename + " not found"};
}

template <typename Value>
Status Filesystem<Value>::PutMany(
    const std::set<std::pair<common::Cid, Value>>& source) noexcept {
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
    const std::set<common::Cid>& source) const noexcept {
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
Status Filesystem<Value>::DeleteMany(
    const std::set<common::Cid>& source) noexcept {
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
