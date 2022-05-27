// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "multiformats/multihash.hpp"

#include <memory>
#include <string>

#include "SHA256.h"
#include "common/status.hpp"
#include "common/utils/prefix_reader.hpp"

// using cognitio::multiformats::multihash::HashType;

namespace cognitio {
namespace common {

Multihash::Multihash() : data_(nullptr){};

Multihash::Data::Data(std::span<uint8_t> &hash) {
  bytes_.reserve(hash.size() + 4);
  uint64_t hash_type = 0x12;

  do {
    uint8_t byte = hash_type & 0x7F;
    hash_type >>= 7;
    if (hash_type != 0) {
      byte |= 0x80;
    }
    bytes_.push_back(byte);
  } while (hash_type > 0);

  bytes_.push_back(static_cast<uint8_t>(hash.size()));
  hash_offset_ = static_cast<uint8_t>(bytes_.size());
  bytes_.insert(bytes_.end(), hash.begin(), hash.end());
}

std::span<uint8_t> Multihash::ToHash(const std::span<uint8_t> &bytes) {
  SHA256 sha;
  std::string bytes_str(bytes.begin(), bytes.end());
  sha.update(bytes_str);

  std::unique_ptr<uint8_t> digest(sha.digest());
  std::span<uint8_t> hash(digest.get(), HASH_LENGTH);
  return hash;
}

Status Multihash::CreateFromBytes(std::span<uint8_t> &bytes) {
  //  hash_bytes = ToHash(bytes);

  SHA256 sha;
  std::string bytes_str(bytes.begin(), bytes.end());
  sha.update(bytes_str);

  uint8_t *digest = sha.digest();
  std::span<uint8_t> hash_bytes(digest, HASH_LENGTH);

  bytes = hash_bytes;

  if (bytes.empty()) {
    return Status::FAILED;
  }

  if (hash_bytes.size() < min_hash_length_) {
    return Status(StatusCode::CANCELLED, "Hash size is short");
  }

  PrefixReader reader;
  if (reader.consume(hash_bytes) != readyState) {
    return Status(CANCELLED, "Hash size is too short");
  }

  if (hash_bytes.empty()) {
    return Status(CANCELLED, "Hash size is too short");
  }

  uint8_t length = *hash_bytes.data();
  std::span<uint8_t> hash = hash_bytes.subspan(1);

  if (length == 0) {
    return Status(CANCELLED, "Zero input length");
  }

  //  if (hash.size() != length) {
  //    return Status(CANCELLED, "Inconsistent length");
  //  }

  //  Multihash(hash);
  data_ = std::make_shared<Data>(hash);

  delete[] digest;
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
