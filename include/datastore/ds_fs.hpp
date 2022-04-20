// // Copyright (c) 2022 NodeOps
// //
// // Distributed under the GNU GPLv3 software license, see the accompanying
// // file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>
//
//#include "datastore.hpp"
//
//#ifndef CGNT_DATASTORE_DS_FS_HPP_
//#define CGNT_DATASTORE_DS_FS_HPP_
//
// namespace cognitio {
// namespace datastore {
// //! This is implementation of datastore filesystem
// //! Responsible for file management in the storage
//
// template <typename Key, typename Value, typename Options>
// class Filesystem : public Datastore<Key, Value, Options> {
// public:
//  void Open();
//  void Close();
//  void Put(const Key& key, const Value& value, const Options& options);
//  void Get(const Key& key, const Options& options);
//  void Delete(const Key& key, const Options& options);
//  void PutMany(const std::set<Key>& source, const Options& options =
//  Options()); void GetMany(const std::set<Key>& source, const Options& options
//  = Options()); void DeleteMany(const std::set<Key>& source,
//                  const Options& options = Options());
//
// private:
//  Options config_;
//  std::filesystem::path path_;
//};
//
//}  // namespace datastore
//}  // namespace cognitio
//
//#endif  // CGNT_DATASTORE_DS_FS_HPP_
