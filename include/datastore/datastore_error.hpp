//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef COGNITIO_DATASTORE_ERROR_HPP
#define COGNITIO_DATASTORE_ERROR_HPP

namespace cognitio {
namespace datastore {

enum class DsError {
  NOT_FOUND = 404,
  UNKNOWN = 405
};

}  // namespace datastore
}  // namespace cognitio

#endif  // COGNITIO_DATASTORE_ERROR_HPP
