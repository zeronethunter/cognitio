// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_COMMON_UTILS_PREFIX_READER_
#define CGNT_COMMON_UTILS_PREFIX_READER_

#include <cstdint>
#include <span>

namespace cognitio {
namespace common {

enum State {
    /// Need more bytes
    needMoreState,
    readyState,

    /// State when uint64_t is overloaded
    overloadState,
    errorState
};

class PrefixReader {
 public:
  PrefixReader() = default;

  State state() const { return state_; };

  uint64_t value() const { return value_; };

  /// \brief Reset reader state
  void reset();

  /// \brief Recieves one byte
  /// \return Reader state
  State consume(uint8_t byte);

  /// \brief Recieves bytes from buffer
  /// In success way cuts off first bytes we were recieved
  /// \return Reader state
  State consume(std::span<uint8_t> &buffer);

 private:
  const uint8_t high_bit_mask_ = 0x80;
  const uint8_t max_bytes_ = 10; 

  uint64_t value_ = 0;           /// Current value
  uint8_t recieved_ = 0;         /// Recieved bytes
  State state_ = needMoreState;  /// Current state
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_COMMON_UTILS_PREFIX_READER_