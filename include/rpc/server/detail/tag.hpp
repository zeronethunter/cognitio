// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_RPC_SERVER_DETAIL_TAG_HPP_
#define CGNT_RPC_SERVER_DETAIL_TAG_HPP_

#include <memory>
#include <ostream>
#include <unordered_map>

namespace cognitio {
namespace rpc {
namespace server {
namespace detail {

enum class TagLabel {
  writing,
  done,
};

struct Tag {
  void* data;
  TagLabel label;

  Tag(void* d, TagLabel l);
};

std::ostream& operator<<(::std::ostream& os, const Tag& tag);

void* MakeTag(void* data, TagLabel label,
               std::unordered_map<void*, std::unique_ptr<Tag>>* tags);
Tag GetTag(void* key, std::unordered_map<void*, std::unique_ptr<Tag>>* tags);

}  // namespace detail
}  // namespace server
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_RPC_SERVER_DETAIL_TAG_HPP_
