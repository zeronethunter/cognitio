//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/identifier.hpp"

namespace cognitio {
namespace kademlia {

Identifier Identifier::FromString(std::string src) {
  try {
    Identifier::numeric_type val =
        boost::lexical_cast<boost::multiprecision::cpp_int>(src)
            .convert_to<Identifier::numeric_type>();
    return Identifier(val);
  } catch (...) {
  }

  return Identifier();
}

Identifier::numeric_type Identifier::value() const { return val_; }

Identifier::numeric_type Identifier::Distance(
    const Identifier::numeric_type& A, const Identifier::numeric_type& B) {
  // Kademlia distance function A xor B
  return A ^ B;
}

Identifier::numeric_type Identifier::distance(
    const Identifier::numeric_type& OtherID) const {
  // Kademlia distance function A xor B
  return val_ ^ OtherID;
}

Identifier::numeric_type Identifier::bucketOf(
    const Identifier::numeric_type& OtherID) const {
  // Get the distance to the other node
  Identifier::numeric_type dist = this->distance(OtherID);

  // Get subtree in which the node should be stored (K-Bucket)
  for (size_t j = 0; j < GUID_SPACE; ++j) {
    if ((dist >> (GUID_SPACE - 1 - j) & 0x1) != 0) {
      return GUID_SPACE - j;
    }
  }

  // The distance between the two nodes are 0 (OtherID == this->val_)
  return 0;
}

Identifier::operator Identifier::numeric_type() const { return val_; }
Identifier::operator std::string() const { return val_.str(); }

std::vector<Identifier> Identifier::GetDirectReferenceIds(
    const Identifier& id) {
  std::vector<Identifier> ret;

  for (unsigned int i = 0; i < GUID_SPACE; i++) {
    Identifier::numeric_type id_value =
        boost::multiprecision::pow((Identifier::numeric_type)2, i);
    ret.push_back(Identifier(id_value));
  }

  return ret;
}

bool operator==(const Identifier& A, const Identifier& B) {
  return A.val_ == B.val_;
}

bool operator!=(const Identifier& A, const Identifier& B) {
  return A.val_ != B.val_;
}

}  // namespace kademlia
}  // namespace cognitio
