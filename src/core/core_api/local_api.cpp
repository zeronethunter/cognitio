// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core_api/local_api.hpp"

#include <fstream>
#include <sstream>
#include <utility>

#include "multiformats/cid.hpp"
#include "common/status.hpp"
#include "common/status_code.hpp"
#include "files/chunker/chunker.hpp"
#include "linked_data/dag_node.hpp"

namespace cognitio {
namespace core {
namespace core_api {

void LocalAPI::Remove(const common::Cid& cid, ResponseEmitter& re) {
  auto err = GetCore()->GetDag()->RemoveNode(cid);
  re.SetStatus(err.error_code());
  if (!err.error_message().empty()) {
    re.Append(err.error_message());
  }
}

void LocalAPI::Get(const common::Cid& cid, ResponseEmitter& re) {
  Status err;
  linked_data::DagNode nd;
  std::tie(err, nd) = GetCore()->GetDag()->GetNode(cid);

  if (!err.error_message().empty()) {
    re.Append(err.error_message());
  } else {
    auto content = nd.GetContent();
    re.Append(std::string(content.begin(), content.end()));
  }
}
void LocalAPI::Add(const std::string& path, ResponseEmitter& re) {
  std::ifstream file(path);
  if (!file.is_open()) {
    re.SetStatus(StatusCode::FAILED, "Unable to open file");
    return;
  }

  std::stringstream buffer_str;
  buffer_str << file.rdbuf();
  std::vector<uint8_t> buffer(buffer_str.str().begin(), buffer_str.str().end());

  // Chunking
  auto chunked_data = files::chunker::chunk_fixed_raw(buffer);

  // Adding node
  common::Cid cid; Status err;
  std::tie(err, cid) = GetCore()->GetDag()->Add(chunked_data);

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
