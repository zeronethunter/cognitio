// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core_api/local_api.hpp"

#include <cstdint>
#include <fstream>
#include <sstream>
#include <utility>

#include "common/status.hpp"
#include "common/status_code.hpp"
#include "common/utils/filesystem.hpp"
#include "files/chunker/chunker.hpp"
#include "linked_data/dag_node.hpp"
#include "multiformats/cid.hpp"

namespace cognitio {
namespace core {
namespace core_api {

void LocalAPI::Remove(const common::Cid& cid, ResponseEmitter& re) {
  auto err = core_->GetDag()->Remove(cid);
  re.SetStatus(err.error_code());
  if (!err.error_message().empty()) {
    re.Append(err.error_message());
  }
}

void LocalAPI::Get(const common::Cid& cid, ResponseEmitter& re) {
  Status err;
  std::vector<uint8_t> nd;
  std::tie(err, nd) = core_->GetDag()->Get(cid);

  if (!err.error_message().empty()) {
    re.Append(err.error_message());
  } else {
    re.Append(std::string(nd.begin(), nd.end()));
  }
}

void LocalAPI::Add(const std::string& path, ResponseEmitter& re) {
  std::vector<uint8_t> buffer;
  Status error;

  std::tie(buffer, error) = common::utils::get_bytes_from(path);
  if (!error.ok()) {
    re.SetStatus(error);
  }

  // Chunking
  auto chunked_data = files::chunker::chunk_fixed_raw(buffer, 1024);

  // Adding node
  common::Cid cid;
  Status err;
  std::tie(err, cid) = core_->GetDag()->Add(chunked_data);

  re.SetStatus(err.error_code());
  if (!err.error_message().empty()) {
    re.Append(err.error_message());
  }

  if (!cid.ToString().empty()) {
    re.Append("\nCID: " + cid.ToString());
  }
}

}  // namespace core_api
}  // namespace core
}  // namespace cognitio
