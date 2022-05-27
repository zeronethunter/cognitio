// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_COMMANDS_LIST_DAEMON_HPP_
#define CGNT_CORE_COMMANDS_LIST_DAEMON_HPP_

#include <grpcpp/impl/service_type.h>

#include <memory>

#include "cli/commands/command.hpp"
#include "common/logger/logger.hpp"
#include "core/context/context.hpp"
#include "core/core_api/api_service.hpp"
#include "core/core_api/local_api.hpp"
#include "exchange/block_swap/rpc/service.hpp"
#include "kademlia/rpc/service.hpp"
#include "proto/ProtoData.grpc.pb.h"

namespace cognitio {
namespace core {
namespace commands {

using namespace cli::commands;

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
    auto api_service = ApiService(*dynamic_cast<LocalAPI*>(ctx.GetAPI().get()));

    std::string api_addr;
    if (env.arguments.contains("--api")) {
      api_addr = env.arguments.at("--api");
    } else {
      api_addr = ctx.GetConfig().Get("api_address");
    }

    std::string dht_addr = ctx.GetConfig().Get("dht_address");

    auto block_swap_service =
        exchange::BlockSwapServiceImpl(ctx.GetCore()->GetRepo());
    auto dht_service =
        kademlia::KademliaServiceImpl(ctx.GetCore()->GetBlockSwap()->GetDht());

    std::vector<rpc::server::ServiceInfo> services(
        {{api_addr, &api_service},
         {dht_addr, &block_swap_service},
         {dht_addr, &dht_service}});

    logger_->debug("API address: {},\nBlockSwap address: {},\nDHT address: {}",
                   api_addr, dht_addr, dht_addr);

    // blocking call
    auto err = ctx.GetCore()->RunDaemon(services);
    re.SetStatus(err);
  }

 private:
  Logger logger_ = createLogger("daemon_cmd");
};

}  // namespace commands
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_COMMANDS_LIST_ADD_HPP_
