// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include <deque>

#include "cli.cpp"
#include "cli/cli.hpp"
#include "common/status.hpp"
#include "core/context/context.hpp"

template class cognitio::cli::Cli<cognitio::core::commands::Context>;
template cognitio::Status
    cognitio::cli::Cli<cognitio::core::commands::Context>::Run<
        std::deque<std::string>>(std::deque<std::string>&) const;
