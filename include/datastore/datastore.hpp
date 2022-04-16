// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <filesystem>
#include <set>

#ifndef CGNT_DATASTORE_DATASTORE_HPP_
#define CGNT_DATASTORE_DATASTORE_HPP_

namespace cognitio {
namespace datastore {

template <typename Key, typename Value, typename Options>
class Datastore {
  std::string config_;
  std::filesystem::path path_;

 public:
  virtual void Open() = 0;   //! Open storage
  virtual void Close() = 0;  //! Close storage
  virtual void Put(
      const Key& key, const Value& value,
      const Options& options) = 0;  //! Put pair{key, value} in storage
  virtual void Get(const Key& key,
                   const Options& options) = 0;  //! Get value by key in storage
  virtual void Delete(
      const Key& key,
      const Options& options) = 0;  //! Delete value by key in storage
  virtual void PutMany(
      const std::set<std::pair<Key, Value>>& source,
      const Options& options =
          Options()) = 0;  //! Put many pairs{key, value} in storage
  virtual void GetMany(
      const std::set<Key>& source,
      const Options& options =
          Options()) = 0;  //! Get many values by keys in storage
  virtual void DeleteMany(
      const std::set<Key>& source,
      const Options& options =
          Options()) = 0;  //! Delete many values by keys in storage
};

}  // namespace datastore
}  // namespace cognitio

#endif  // CGNT_DATASTORE_DATASTORE_HPP_
