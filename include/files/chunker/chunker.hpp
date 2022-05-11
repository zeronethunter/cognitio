// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CONGITIO_CHUNKER_CHUNKER_HPP_
#define CONGITIO_CHUNKER_CHUNKER_HPP_

#include <iostream>

#include "common/status.hpp"
#include "set"
#include "vector"

namespace cognitio {
namespace files {
namespace chunker {

/**
 *  @brief  Function to chunk raw of bytes into blocks of data with setted size.
 *
 *  @tparam  Buffer type of container for bytes.
 *  @param  blocks container of bytes.
 *  @param  max_width max size of chunked block, default = 262144 bytes.
 *
 *  @return vector of chunked blocks.
 */
template <typename Buffer>
std::vector<Buffer> chunk_fixed_raw(const Buffer& blocks,
                                    const size_t max_width = 262144);

}  // namespace chunker
}  // namespace files
}  // namespace cognitio

#endif  // CONGITIO_CHUNKER_CHUNKER_HPP_
