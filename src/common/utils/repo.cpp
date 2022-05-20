// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "common/utils/repo.hpp"

namespace cognitio {
namespace common {
namespace utils {

std::string GetDefaultRepoPath() {
  auto repo_home_char = std::getenv("XDG_DATA_HOME");
  std::string repo_home;
  if (!repo_home_char) {
      repo_home = "~/.cognitio";
  } else {
      repo_home = std::string(repo_home_char) + "/cognitio";
  }

  return repo_home;
}

}  // namespace utils
}  // namespace common
}  // namespace cognitio
