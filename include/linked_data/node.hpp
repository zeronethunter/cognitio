//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_LINKED_DATA_NODE_HPP_
#define CGNT_LINKED_DATA_NODE_HPP_

#include "common/multiformats/cid.hpp"
#include "linked_data/link.hpp"

namespace cognitio {
namespace linked_data {

class Node {
 public:
  virtual ~Node() = default;

  virtual common::Cid GetCid() const = 0;

  virtual std::vector<uint8_t> GetBytes() const = 0;

  virtual size_t Size() const = 0;

  virtual Status AddChild(const std::string &name,
                          std::shared_ptr<Node> node) = 0;

  virtual std::unique_ptr<Node> GetLink(const std::string &name) const = 0;

  virtual void RemoveLink(const std::string &name) = 0;

  virtual void AddLink(const Link &link) = 0;

  virtual std::vector<std::unique_ptr<Link>> GetLinks() const = 0;
};

}  // namespace linked_data
}  // namespace cognitio

#endif  // CGNT_LINKED_DATA_NODE_HPP_