// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_LINK_HPP_
#define CGNT_LINKED_DATA_LINK_HPP_

#include <string>

#include "common/multiformats/cid.hpp"

namespace cognitio {
namespace linked_data {

class Link {
 public:
  virtual ~Link() = default;

  virtual std::string GetName() const = 0;

  virtual common::Cid GetCid() const = 0;

  virtual size_t GetSize() const = 0;
};

}  // namespace common
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_LINK_HPP_