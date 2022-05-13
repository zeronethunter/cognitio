//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CGNT_EXCHANGE_BLOCK_SERVICE_BLOCK_SERVICE_HPP_
#define CGNT_EXCHANGE_BLOCK_SERVICE_BLOCK_SERVICE_HPP_

#include "common/block/block.hpp"
#include "common/status.hpp"
#include "common/utils/repo.hpp"
#include "exchange/block_swap/block_swap.hpp"
#include "repo/repo.hpp"

namespace cognitio {
namespace exchange {

template <typename Value>
class BlockService {
 public:
  Status Open(const std::filesystem::path& path =
                  std::filesystem::path(common::utils::GetDefaultRepoPath()) /
                  "blocks",
              bool is_daemon_opened = false) noexcept;

  Status Close() noexcept;

  std::filesystem::path Root() const noexcept;

  Status Put(const common::Cid& key, const Value& value) noexcept;

  Value Get(const common::Cid& key) const noexcept;

  Status Delete(const common::Cid& key) noexcept;

  bool Has(const common::Cid& key) const noexcept;

  Status PutMany(
      const std::set<std::pair<common::Cid, Value>>& source) noexcept;

  std::set<Value> GetMany(const std::set<common::Cid>& source) const noexcept;

  Status DeleteMany(const std::set<common::Cid>& source) noexcept;

 private:
  bool closed_ = true;
  bool is_daemon_opened_ = false;

  std::unique_ptr<BlockSwap> block_swap_;
  std::unique_ptr<repo::Repo<std::string>> repo_;

  common::logger::Logger logger_ =
      common::logger::createLogger("BlockService logger");
};

}  // namespace exchange
}  // namespace cognitio

#endif  // CGNT_EXCHANGE_BLOCK_SERVICE_BLOCK_SERVICE_HPP_
