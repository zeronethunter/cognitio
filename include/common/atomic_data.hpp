// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_ATOMIC_DATA_HPP_
#define CGNT_COMMON_ATOMIC_DATA_HPP_
#include <condition_variable>
#include <mutex>

namespace cognitio {
namespace common {

//! \brief Owns complex data that can be accessed in a thread safe way.
//!
//! Example:
//!     struct MyComplexData {
//!         int thing1;
//!         bool thing2;
//!         std::vector<double> more_things;
//!         OtherStruct complex_thing;
//!     };
//!
//!     AtomicData<MyComplexData> shared_data;
//!
//!     ... Later, in different threads
//!
//!     shared_data.use_safely([] (MyComplexData& data) {
//!         // Do things with 'data' here:
//!         ...
//!     });
//!     ...
template <typename T>
class AtomicData {
 public:
  explicit AtomicData(T&& data);

  template <typename... Args>
  explicit AtomicData(Args&&... args);

  //! \brief Use the data in a thread safe manner.
  template <typename Func>
  auto UseSafely(const Func& func);

  template <typename Func>
  auto UseSafely(const Func& func) const;

  //! \brief Wait for 'notify_one' or 'notify_all' to be called on this data
  //!        structure before using the data in a thread safe manner.
  template <typename Pred, typename Func>
  auto WaitToUseSafely(const Pred& predicate, const Func& func);

  template <typename Pred, typename Func>
  auto WaitToUseSafely(const Pred& predicate, const Func& func) const;

  //! \brief Allow one 'wait_to_use_safely' function to continue.
  void NotifyOne();

  //! \brief Allow all 'wait_to_use_safely' functions to continue.
  void NotifyAll();

 private:
  mutable std::mutex lock_;  //!< mutable so it can be used with const functions
  std::condition_variable condition_;
  T data_;
};

template <typename T>
AtomicData<T>::AtomicData(T&& data) : data_(std::forward<T>(data)) {
}

template <typename T>
template <typename... Args>
AtomicData<T>::AtomicData(Args&&... args) : data_(std::forward<Args>(args)...) {
}

template <typename T>
template <typename Func>
auto AtomicData<T>::UseSafely(const Func& func) {
  std::lock_guard<std::mutex> scoped_lock(lock_);
  return func(data_);
}

template <typename T>
template <typename Func>
auto AtomicData<T>::UseSafely(const Func& func) const {
  std::lock_guard<std::mutex> scoped_lock(lock_);
  return func(data_);
}

template <typename T>
template <typename Pred, typename Func>
auto AtomicData<T>::WaitToUseSafely(const Pred& predicate, const Func& func) {
  std::unique_lock<std::mutex> unlockable_lock(lock_);
  condition_.wait(unlockable_lock, [&] { return predicate(data_); });
  return func(data_);
}

template <typename T>
template <typename Pred, typename Func>
auto AtomicData<T>::WaitToUseSafely(const Pred& predicate,
                                    const Func& func) const {
  std::unique_lock<std::mutex> unlockable_lock(lock_);
  condition_.wait(unlockable_lock, [&] { return predicate(data_); });
  return func(data_);
}

template <typename T>
void AtomicData<T>::NotifyOne() {
  condition_.notify_one();
}

template <typename T>
void AtomicData<T>::NotifyAll() {
  condition_.notify_all();
}

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_ATOMIC_DATA_HPP_
