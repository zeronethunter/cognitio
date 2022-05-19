//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "files/chunker/chunker.hpp"

namespace cognitio {
namespace files {
namespace chunker {

template <typename Buffer>
std::vector<Buffer> chunk_fixed_raw(const Buffer& buffer,
                                    const size_t max_width) noexcept {
  size_t size = buffer.size();
  size_t i = 0;
  std::vector<Buffer> result;
  while (size >= max_width) {
    result.push_back(
        Buffer(std::begin(buffer) + i, std::begin(buffer) + max_width + i - 1));
    i += max_width;
    size -= max_width;
  }

  result.push_back(Buffer(std::begin(buffer) + i, std::end(buffer)));

  return result;
}

template std::vector<std::vector<uint8_t>> chunk_fixed_raw(
    const std::vector<uint8_t>& buffer, const size_t max_width) noexcept;

}  // namespace chunker
}  // namespace files
}  // namespace cognitio
