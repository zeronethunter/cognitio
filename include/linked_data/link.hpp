// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_LINK_HPP_
#define CGNT_LINKED_DATA_LINK_HPP_

#include <string>

#include "multiformats/cid.hpp"
#include "proto/ProtoData.pb.h"
#include "common/status.hpp"

namespace cognitio {
namespace linked_data {

class Link {
 public:
  Link() = default;
  
  Link(const common::Cid &cid, const std::string &str, size_t size)
      : cid_(cid), name_(str), size_(size){};

  common::Cid GetCid() const {return cid_; };

  std::string GetName() const { return name_; };

  size_t GetSize() const { return size_; };

  Status DecodeProtoLink(const Link &link);

  std::unique_ptr<Link> EncodeProtoLink() const;

 private:
  common::Cid cid_;
  std::string name_;
  size_t size_;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_LINK_HPP_
