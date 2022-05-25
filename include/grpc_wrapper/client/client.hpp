// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_GRPC_WRAPPER_CLIENT_CLIENT_HPP_
#define CGNT_GRPC_WRAPPER_CLIENT_CLIENT_HPP_

#include <memory>
#include <string>

namespace cognitio {
namespace rpc {
namespace client {

template <class Service>
class Client {
 public:
  typedef std::shared_ptr<typename Service::Stub> StubPtr;

  explicit Client() = default;
  explicit Client(const std::string& addr);
  void ChangeServer(const std::string& address);
  const std::string& GetServerAddress() const { return host_addr_; }
  StubPtr GetStub() noexcept { return stub_; }
  ~Client() = default;

 private:
  std::string host_addr_;
  StubPtr stub_;
};

}  // namespace client
}  // namespace rpc
}  // namespace cognitio

#endif  // CGNT_GRPC_WRAPPER_CLIENT_CLIENT_HPP_
