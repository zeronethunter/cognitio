// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "cli/commands/response_emitter.hpp"

namespace cognitio {
namespace cli {
namespace commands {

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

void ResponseEmitter::Append(const std::string& data) {
  *proto_.mutable_data() += data;
}

void ResponseEmitter::SetStatus(StatusCode code, std::string msg) {
  proto_.mutable_status()->set_statuscode(to_underlying(code));
  proto_.mutable_status()->set_statusmsg(msg);
}

}  // namespace commands
}  // namespace cli
}  // namespace cognitio
