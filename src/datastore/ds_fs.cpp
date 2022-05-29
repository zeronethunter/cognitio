// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "datastore/ds_fs.hpp"

#include <fstream>

#include "common/status.hpp"

namespace cognitio {
namespace datastore {

template <typename Value>
Status Filesystem<Value>::Open(const std::filesystem::path &path) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  path_ = path;
  if (!std::filesystem::exists(path)) {
    if (std::filesystem::create_directory(path_)) {
      return Status::OK;
    }
    return {StatusCode::FAILED, "Can't open storage."};
  }

  return Status::FAILED;
}

template <typename Value>
Status Filesystem<Value>::Close() noexcept {}

template <typename Value>
Status Filesystem<Value>::Put(const common::Cid &key,
                              const Value &value) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  std::string filename = key.ToString();
  if (std::filesystem::exists(path_ / filename)) {
    return {StatusCode::ALREADY_EXISTS, " already exists"};
  }
  std::fstream file;
  file.open((path_ / filename).string(),
            std::ios::out | std::ios::trunc | std::ios::binary);

  if (!file.is_open()) {
    return {StatusCode::CANCELLED, "Can not create " + filename};
  }

  char *bytes = makeCharFromData(value);
  file.write(bytes, value.size());
  file.close();

  delete[] bytes;
  return Status::OK;
}

template <typename Value>
std::pair<Status, Value> Filesystem<Value>::Get(
    const common::Cid &key) noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  std::string filename = key.ToString();
  if (!std::filesystem::exists(path_ / filename)) {
    return std::pair<Status, Value>({StatusCode::NOT_FOUND, " not found"},
                                    Value());
  }

  std::fstream file;
  file.open((path_ / filename).string(), std::ios::in | std::ios::binary);

  if (!file.is_open()) {
    return std::pair<Status, Value>(
        {StatusCode::CANCELLED, "Can not create " + filename}, Value());
  }

  std::stringstream bytes;

  bytes << file.rdbuf();

  std::string str_bytes = bytes.str();

  file.close();

  return std::pair<Status, Value>(Status::OK,
                                  {str_bytes.cbegin(), str_bytes.cend()});
}

template <typename Value>
Status Filesystem<Value>::Delete(const common::Cid &key)

    noexcept {
  const std::lock_guard<std::mutex> lock(mutex_);
  std::string filename = key.ToString();
  if (std::filesystem::remove(path_ / filename)) {
    return Status::OK;
  }
  return {StatusCode::NOT_FOUND, filename + " not found"};
}

template <typename Value>
Status Filesystem<Value>::PutMany(
    const std::set<std::pair<common::Cid, Value>> &source) noexcept {
  for (const auto &input : source) {
    Status err = Put(input.first, input.second);
    if (!err.ok()) {
      return err;
    }
  }
  return Status::OK;
}

template <typename Value>
std::pair<Status, std::set<Value>> Filesystem<Value>::GetMany(
    const std::set<common::Cid> &source) noexcept {
  std::set<Value> result;
  for (const auto &key : source) {
    std::pair<Status, Value> it = Get(key);
    result.insert(it.second);
    if (!it.first.ok()) {
      return std::pair<Status, std::set<Value>>(it.first, std::set<Value>());
    }
  }
  return std::pair<Status, std::set<Value>>(Status::OK, result);
}

template <typename Value>
Status Filesystem<Value>::DeleteMany(
    const std::set<common::Cid> &source) noexcept {
  for (const auto &key : source) {
    Status err = Delete(key);
    if (!err.ok()) {
      return err;
    }
  }
  return Status::OK;
}

template <typename Value>
char *Filesystem<Value>::makeCharFromData(const Value &value) const noexcept {
  char *result = new char[value.size()];
  for (size_t i = 0; i < value.size(); ++i) {
    result[i] = value[i];
  }
  return result;
}

}  // namespace datastore
}  // namespace cognitio
