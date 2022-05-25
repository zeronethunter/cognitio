// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "core/core_api/remote_api.hpp"

#include <grpcpp/client_context.h>

#include "common/logger/logger.hpp"
#include "common/status.hpp"
#include "core/core_api/api_service.hpp"
#include "grpc_wrapper/client/client.hpp"
#include "proto/api.pb.h"

namespace cognitio {
namespace core {
namespace core_api {

RemoteAPI::RemoteAPI(const std::string& host_addr)
    : client_(std::make_unique<Client<CoreApiService>>(host_addr)),
      logger_(common::logger::createLogger("remote_api")) {}

void RemoteAPI::Remove(const common::Cid& cid, ResponseEmitter& re) {
  StringRequest req;
  req.set_data(cid.ToString());

  ProtoResponse resp;
  grpc::ClientContext context;

  client_->GetStub()->Remove(&context, req, &resp);
  re.ReplaceProto(resp);
}

void RemoteAPI::Get(const common::Cid& cid, ResponseEmitter& re) {
  StringRequest req;
  req.set_data(cid.ToString());

  ProtoResponse resp;
  grpc::ClientContext context;

  client_->GetStub()->Get(&context, req, &resp);
  re.ReplaceProto(resp);
}
void RemoteAPI::Add(const std::string& path, ResponseEmitter& re) {
  StringRequest req;
  req.set_data(path);

  ProtoResponse resp;
  grpc::ClientContext context;

  client_->GetStub()->Add(&context, req, &resp);
  re.ReplaceProto(resp);
}

Status RemoteAPI::TryPing() const noexcept {
  StringRequest req, resp;
  grpc::ClientContext context;
  client_->GetStub()->Ping(&context, req, &resp);
  return !resp.data().empty() ? Status::OK : Status::FAILED;
}

}  // namespace core_api
}  // namespace core
}  // namespace cognitio
