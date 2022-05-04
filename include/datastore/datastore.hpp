// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_DATASTORE_DATASTORE_HPP_
#define CGNT_DATASTORE_DATASTORE_HPP_

#include <filesystem>
#include <set>

#include "common/status.hpp"

namespace cognitio {
namespace datastore {

template <typename Key, typename Value, typename ErrCode>
class Datastore {
 public:
  virtual ErrCode Open(const std::filesystem::path& path) = 0;   //! Open storage
  virtual ErrCode Close() = 0;  //! Close storage
  virtual ErrCode Put(
      const Key& key,
      const Value& value) = 0;  //! Put pair{key, value} in storage
  virtual std::pair<ErrCode, Value> Get(
      const Key& key) = 0;  //! Get value by key in storage
  virtual ErrCode Delete(
      const Key& key) = 0;  //! Delete value by key in storage
  virtual ErrCode PutMany(
      const std::set<std::pair<Key, Value>>&
          source) = 0;  //! Put many pairs{key, value} in storage
  virtual std::pair<ErrCode, std::set<Value>> GetMany(
      const std::set<Key>& source) = 0;  //! Get many values by keys in storage
  virtual ErrCode DeleteMany(
      const std::set<Key>&
          source) = 0;  //! Delete many values by keys in storage
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DATASTORE_HPP_
