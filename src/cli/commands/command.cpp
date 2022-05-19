// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/commands/command.hpp"

#include <algorithm>

namespace cognitio {
namespace cli {
namespace commands {

template <class Context>
typename Command<Context>::CmdPtr Command<Context>::GetSubCmd(
    const std::string& name) const {
  auto it = std::find_if(
      sub_commands_.begin(), sub_commands_.end(),
      [&name](const CmdPtr ptr) { return ptr->GetMeta().GetName() == name; });

  return it == sub_commands_.end() ? nullptr : *it;
}


template <class Context>
std::vector<std::string> Command<Context>::GetSubCmdsNames() const noexcept {
  std::vector<std::string> vec;
  for (const auto& x : sub_commands_) {
    vec.push_back(x->GetMeta().GetName());
  }

  return vec;
}

}  // namespace commands
}  // namespace cli
}  // namespace cognitio
