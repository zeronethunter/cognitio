// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_API_API_SERVICE_HPP_
#define CGNT_CORE_CORE_API_API_SERVICE_HPP_

#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include <grpcpp/support/status.h>

#include <memory>

#include "cli/commands/response_emitter.hpp"
#include "common/logger/logger.hpp"
#include "core/core_api/core_api.hpp"
#include "core/core_api/local_api.hpp"
#include "multiformats/cid.hpp"
#include "proto/api.grpc.pb.h"
#include "proto/api.pb.h"

namespace cognitio {
namespace core {
namespace core_api {

using namespace cli::commands;

class ApiService final : public CoreApiService::Service {
 public:
  explicit ApiService(LocalAPI& api) : api_(api) {
    logger_ = common::logger::createLogger("api_service");
  }

  grpc::Status Add([[maybe_unused]] grpc::ServerContext* context,
                   const StringRequest* request, ProtoResponse* resp) override {
    logger_->info("Recieved request for adding blocks");
    ResponseEmitter re;
    api_.Add(request->data(), re);
    *resp = re.GetProto();
    return grpc::Status::OK;
  }

  grpc::Status Get([[maybe_unused]] grpc::ServerContext* context,
                   const StringRequest* request, ProtoResponse* resp) override {
    logger_->info("Recieved request for getting blocks");
    ResponseEmitter re;
    api_.Get(common::Cid(request->data()), re);
    *resp = re.GetProto();
    return grpc::Status::OK;
  }

  grpc::Status Remove([[maybe_unused]] grpc::ServerContext* context,
                      const StringRequest* request,
                      ProtoResponse* resp) override {
    logger_->info("Recieved request for removing blocks");
    ResponseEmitter re;
    api_.Remove(common::Cid(request->data()), re);
    *resp = re.GetProto();
    return grpc::Status::OK;
  }

  grpc::Status Ping([[maybe_unused]] grpc::ServerContext* context,
                    [[maybe_unused]] const StringRequest* request,
                    StringRequest* resp) override {
    logger_->info("Recieved ping");
    resp->set_data(std::string("World"));
    return grpc::Status::OK;
  }

 private:
  LocalAPI& api_;
  common::logger::Logger logger_;
};

}  // namespace core_api
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_API_API_SERVICE_HPP_
