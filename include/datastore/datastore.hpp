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

/**
 *  @brief  This is implementation of Datastore interface.
 *
 *  Interface to implement other Datastores: Filestore and DBStore.
 *
 *  @tparam  Key type of key.
 *  @tparam  Value type of value to put in local storage.
 *  @tparam  ErrCode type of Error handling class.
 */
template <typename Key, typename Value, typename ErrCode>
class Datastore {
 public:
  /**
   *  @brief  Open storage.
   *
   *  Opens Datastore in certain path.
   *
   *  @param path where to open storage.
   */
  virtual ErrCode Open(const std::filesystem::path& path) = 0;
  /**
   *  @brief  Close storage.
   *
   *  Closes Datastore in a predefined path.
   */
  virtual ErrCode Close() = 0;
  /**
   *  @brief  Put value in storage.
   *
   *  @param key the key that will be used to put value.
   *  @param value value to put in store by key.
   */
  virtual ErrCode Put(const Key& key, const Value& value) = 0;
  /**
   *  @brief  Get value by key from storage.
   *
   *  @param key the key that will be used to get value.
   *
   *  @return pair of ErrCode and Value types.
   */
  virtual std::pair<ErrCode, Value> Get(const Key& key) = 0;
  /**
   *  @brief  Delete value by key in storage.
   *
   *  @param key the key that will be used to delete value.
   */
  virtual ErrCode Delete(const Key& key) = 0;
  /**
   *  @brief  Put set of values by keys in storage.
   *
   *  @param source set of pairs<key, value> that will be putted in storage.
   */
  virtual ErrCode PutMany(const std::set<std::pair<Key, Value>>& source) = 0;
  /**
   *  @brief  Get set of values by keys from storage.
   *
   *  @param source set of keys of values to get from storage.
   */
  virtual std::pair<ErrCode, std::set<Value>> GetMany(
      const std::set<Key>& source) = 0;
  /**
   *  @brief  Delete set of values by keys from storage.
   *
   *  @param source set of keys of values to delete from storage.
   */
  virtual ErrCode DeleteMany(const std::set<Key>& source) = 0;
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DATASTORE_HPP_
