// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_CORE_CORE_API_REMOTE_API_HPP_
#define CGNT_CORE_CORE_API_REMOTE_API_HPP_

#include <memory>

#include "common/logger/logger.hpp"
#include "core/core_api/api_service.hpp"
#include "core/core_api/core_api.hpp"
#include "grpc_wrapper/client/client.hpp"
#include "proto/api.grpc.pb.h"

namespace cognitio {
namespace core {
namespace core_api {

using namespace rpc::client;

class RemoteAPI final : public CoreAPI {
 public:
  typedef std::unique_ptr<Client<CoreApiService>> ClientPtr;

  explicit RemoteAPI(const std::string& host_addr);
  void Remove(const common::Cid& cid, ResponseEmitter& re) override;
  void Get(const common::Cid& cid, ResponseEmitter& re) override;
  void Add(const std::string& path, ResponseEmitter& re) override;
  Status TryPing() const noexcept;
  ~RemoteAPI() = default;

 private:
  ClientPtr client_;
  common::logger::Logger logger_;
};

}  // namespace core_api
}  // namespace core
}  // namespace cognitio

#endif  // CGNT_CORE_CORE_API_LOCAL_API_HPP_
