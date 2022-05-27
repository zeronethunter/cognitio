// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_NODE_HPP
#define CGNT_LINKED_DATA_NODE_HPP

#include "multiformats/cid.hpp"

namespace cognitio {
namespace common {

Cid::Cid() : str_cid_(), bytes_view_(), content_type_(), content_address_() {}

Cid::Cid(std::span<uint8_t> &bytes) {
  content_address_.CreateFromBytes(bytes);
  str_cid_ = content_address_.ToHex();
  bytes_view_ = content_address_.ToBuffer();
  content_type_ = CodeType::SHA2_256;
}

Cid::Cid(const std::vector<uint8_t> &bytes) {
  std::vector<uint8_t> copy_to_vec(bytes);
  std::span<uint8_t> sp(copy_to_vec);
  content_address_.CreateFromBytes(sp);
  str_cid_ = content_address_.ToHex();
  bytes_view_ = content_address_.ToBuffer();
  content_type_ = CodeType::SHA2_256;
}

Cid::Cid(std::string_view str_view)
    : str_cid_(str_view), bytes_view_(str_view.begin(), str_view.end()) {
  //  content_address_.CreateFromHex(str_view);
  //  str_cid_ = content_address_.ToHex();
  //  bytes_view_ = content_address_.ToBuffer();
  content_type_ = CodeType::SHA2_256;
}

Cid::Cid(CodeType code_type, const Multihash &content_type)
    : content_type_(code_type), content_address_(content_type) {
  str_cid_ = content_address_.ToHex();
  bytes_view_ = content_address_.ToBuffer();
}

Cid::Cid(const Cid &other)
    : str_cid_(other.str_cid_),
      bytes_view_(other.bytes_view_),
      content_type_(other.content_type_),
      content_address_(other.content_address_) {}

Cid::Cid(Cid &&other)
    : str_cid_(other.str_cid_),
      bytes_view_(other.bytes_view_),
      content_type_(other.content_type_),
      content_address_(std::move(other.content_address_)) {}

Cid &Cid::operator=(const Cid &other) {
  if (this != &other) {
    str_cid_ = other.str_cid_;
    bytes_view_ = other.bytes_view_;
    content_address_ = other.content_address_;
    content_type_ = other.content_type_;
  }
  return *this;
}

Cid &Cid::operator=(Cid &&other) {
  if (this != &other) {
    str_cid_ = other.str_cid_;
    bytes_view_ = other.bytes_view_;
    content_address_ = std::move(other.content_address_);
    content_type_ = other.content_type_;
  }
  return *this;
}

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_NODE_HPP
