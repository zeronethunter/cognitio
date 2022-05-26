// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_LIST_DAEMON_HPP_
#define CGNT_CORE_COMMANDS_LIST_DAEMON_HPP_

#include <grpcpp/impl/service_type.h>

#include <csignal>
#include <memory>

#include "cli/commands/command.hpp"
#include "core/context/context.hpp"
#include "core/core_api/api_service.hpp"
#include "core/core_api/local_api.hpp"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

static std::atomic<bool> shutdown = false;
void signal_handler([[maybe_unused]] int signal) { shutdown = true; }

void listen_shutdown(Context::CorePtr core) {
  while (true) {
    if (shutdown) {
      core->Shutdown();
      break;
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));
  }
}

class DaemonMeta : public CmdMeta {
 public:
  DaemonMeta() : CmdMeta(std::string("daemon"), {}, false, true) {}
};

class DaemonCmd : public Command<Context> {
 public:
  DaemonCmd() : Command(DaemonMeta()){};
  void PrintHelp([[maybe_unused]] std::ostream& out) override {}

  void Run(Context& ctx, [[maybe_unused]] CmdEnv& env,
           ResponseEmitter& re) override {
    // Server initialization
    auto api_service_ =
        ApiService(*dynamic_cast<LocalAPI*>(ctx.GetAPI().get()));
    std::string api_addr = ctx.GetConfig().Get("api_address");
    std::vector<rpc::server::ServiceInfo> services({{api_addr, &api_service_}});
    auto err = ctx.GetCore()->RunDaemon(services);
    if (err.ok()) {
      std::signal(SIGINT, signal_handler);
      auto shutdown_thread = std::thread(&listen_shutdown, ctx.GetCore());
      shutdown_thread.join();
    }

    re.SetStatus(err);
  }

 private:
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_LIST_ADD_HPP_
