//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "repo/repo.hpp"

namespace cognitio {
namespace repo {

Repo::Repo(datastore::Filesystem<std::vector<uint8_t>> const& root) {
  root_ = root;
  closed_ = false;
}

Repo Repo::CreateRepo(const std::filesystem::path& path) {
  datastore::Filesystem<std::vector<uint8_t>> fs;
  fs.Open(path);
  return Repo(fs);
}

Status Repo::Init() {
  if (closed_) {
    return Status(StatusCode::CANCELLED, "You should create Repo first");
  }

  blockstorage::Blockstorage blocks;
  Status err = blocks.Open(root_.Root() / "blocks");

  if (!err.ok()) {
    return err;
  }

  blocks_ = blocks;
  // TODO: config creation
}

}  // namespace repo
}  // namespace cognitio
