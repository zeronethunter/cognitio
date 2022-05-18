// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "multiformats/multihash.hpp"

#include "SHA256.h"
#include "common/status.hpp"
#include "common/utils/prefix_reader.hpp"

// using cognitio::multiformats::multihash::HashType;

namespace cognitio {
namespace common {

Multihash::Multihash() : data_(nullptr){};

void Multihash::ToHash(const std::span<uint8_t> &bytes) {
  SHA256 sha;
  sha.update(reinterpret_cast<const uint8_t *>(bytes.front()), bytes.size());
}

Status Multihash::CreateFromBytes(std::span<uint8_t> &bytes) {
  ToHash(bytes);

  if (bytes.size() < min_hash_length_) {
    return Status(StatusCode::CANCELLED, "Hash size is short");
  }

  PrefixReader reader;
  if (reader.consume(bytes) != readyState) {
    return Status(StatusCode::CANCELLED, "Hash size is too short");
  }

  if (bytes.empty()) {
    return Status(StatusCode::CANCELLED, "Hash size is too short");
  }

  uint8_t length = *bytes.data();
  std::span<uint8_t> hash = bytes.subspan(1);

  if (length == 0) {
    return Status(CANCELLED, "Zero input length");
  }
  if (hash.size() != length) {
    return Status(CANCELLED, "Inconsistent length");
  }

  return Status::OK;
}

Status Multihash::CreateFromHex(std::string_view &hex) {
  std::vector<uint8_t> buffer = unhex(hex);
  std::span<uint8_t> b(buffer);
  return CreateFromBytes(b);
}

bool Multihash::operator==(const Multihash &other) {
  if (data_ == other.data_) {
    return true;
  }
  const auto &first = data();
  const auto &second = other.data();
  return first.bytes_ == second.bytes_;
}

bool Multihash::operator!=(const Multihash &other) {
  return !(operator==(other));
}

bool Multihash::operator<(const Multihash &other) {
  const auto &first = data();
  const auto &second = other.data();
  return first.bytes_ < second.bytes_;
}

}  // namespace common
}  // namespace cognitio
