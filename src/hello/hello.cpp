// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "hello/hello.hpp"

void hello(std::ostream &out, const std::string& str) {
  out << str << std::endl;
}
