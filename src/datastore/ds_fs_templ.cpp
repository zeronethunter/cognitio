// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <vector>

#include "datastore/ds_fs.hpp"
#include "ds_fs.cpp"

template class cognitio::datastore::Filesystem<std::vector<uint8_t>>;
template class cognitio::datastore::Filesystem<std::string>;
