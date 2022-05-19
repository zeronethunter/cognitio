// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/context/context.hpp"
#include "cli/commands/command.hpp"
#include "command.cpp"

template class cognitio::cli::commands::Command<cognitio::core::commands::Context>;
