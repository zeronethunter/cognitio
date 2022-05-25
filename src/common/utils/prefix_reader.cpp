// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/utils/prefix_reader.hpp"

namespace cognitio {
namespace common {

void PrefixReader::reset() {
  value_ = 0;
  recieved_ = 0;
  state_ = needMoreState;
}

State PrefixReader::consume(uint8_t byte) {
  if (state_ == needMoreState) {
    bool is_next_byte = ((byte & high_bit_mask_) != 0);
    uint64_t temp = byte & ~high_bit_mask_;

    if (++recieved_ == max_bytes_) {
      if (temp > 1 || is_next_byte) {
        state_ = overloadState;
        return state_;
      }
    } else {
      temp <<= 7 * (recieved_ - 1);
    }

    value_ += temp;
    if (!is_next_byte) {
      state_ = readyState;
    } else if (state_ == readyState) {
      state_ = errorState;
    }
    
    return state_;
  }
}

State PrefixReader::consume(std::span<uint8_t> &buffer) {
  size_t consumed_counter = 0;
  State state(state_);

  for (uint8_t element : buffer) {
    ++consumed_counter;
    state = consume(element);
    if (state != needMoreState) {
      break;
    }
  }

  if (consumed_counter > 0 && (readyState || needMoreState)) {
    buffer = buffer.subspan(consumed_counter);
  }
  return state;
}

}  // namespace common
}  // namespace cognitio
