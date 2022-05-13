// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_LOCAL_EMITTER_HPP
#define CGNT_CORE_COMMANDS_LOCAL_EMITTER_HPP

#include <ostream>

#include "cli/commands/response_emitter.hpp"

namespace cognitio {
namespace core {
namespace commands {
using namespace cli::commands;
class LocalEmitter : public ResponseEmitter {
 public:
  explicit LocalEmitter(std::ostream& out)
      : ResponseEmitter(), out_(out), err_(err) {}
  ~LocalEmitter() = default;

  void Emit() override { out_ << GetProto().data(); }

 private:
  std::ostream& out_;
};
}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_LOCAL_EMITTER_HPP
