//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_KADEMLIA_IDENTIFIER_HPP_
#define CGNT_KADEMLIA_IDENTIFIER_HPP_

#include <boost/multiprecision/cpp_int.hpp>

#include "kademlia/sys_def.hpp"

namespace cognitio {
namespace kademlia {

class Identifier {
 public:
  typedef boost::multiprecision::number<boost::multiprecision::cpp_int_backend<
      GUID_SPACE, GUID_SPACE, boost::multiprecision::unsigned_magnitude,
      boost::multiprecision::unchecked, void>>
      numeric_type;

  Identifier() : val_(0) {}
  Identifier(Identifier::numeric_type val) : val_(val) {}
  ~Identifier() = default;

  static Identifier FromString(std::string src);
  Identifier::numeric_type value() const;
  static Identifier::numeric_type Distance(const Identifier::numeric_type& A,
                                           const Identifier::numeric_type& B);
  Identifier::numeric_type distance(
      const Identifier::numeric_type& OtherID) const;

  Identifier::numeric_type bucketOf(
      const Identifier::numeric_type& OtherID) const;

  friend bool operator==(const Identifier& A, const Identifier& B);
  friend bool operator!=(const Identifier& A, const Identifier& B);

  operator Identifier::numeric_type() const;
  operator std::string() const;

  static std::vector<Identifier> GetDirectReferenceIds(const Identifier& id);

 private:
  Identifier::numeric_type val_;
};

}  // namespace kademlia
}  // namespace cognitio

#endif  // CGNT_KADEMLIA_IDENTIFIER_HPP_
