// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_COMMAND_HPP_
#define CGNT_CORE_COMMANDS_COMMAND_HPP_

template <class Context>
class RootCmd : public Command<Context> {
 public:
  Status Run([[maybe_unused]] Context& ctx, [[maybe_unused]] const CmdEnv& env,
             [[maybe_unused]] ResponseEmitter& re) override {}
};

template <class Context>
RootCmd<Context> MakeCommands();
