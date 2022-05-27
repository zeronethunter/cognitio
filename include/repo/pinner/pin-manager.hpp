// Copyright (c) 2022 NodeOps
//
// Distributed under the GNU GPLv3 software license, see the accompanying
// file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#ifndef CNGT_REPO_PINNER_PIN_MANAGER_HPP_
#define CNGT_REPO_PINNER_PIN_MANAGER_HPP_

#include <cstring>
#include <filesystem>
#include <fstream>
#include <set>
#include <utility>

#include "common/logger/logger.hpp"
#include "datastore/ds_fs.hpp"
#include "repo/block_storage/block_storage.hpp"

namespace cognitio {
namespace repo {
namespace pinner {
/**
 *  @brief  Pin Manager.
 *
 *  Pins and Unpins blocks, so the Garbage Collector doesn't remove them
 */
class PinManager {
  typedef std::set<std::string> Pins;
  typedef std::filesystem::path PinStore;

 public:
  PinManager() = delete;

  explicit PinManager(PinStore& pinstore, const Pins& pins = Pins()) noexcept
      : pinstore_(std::move(pinstore)),
        logger_(common::logger::createLogger("pinner")) {
    if (!pins.empty()) {
      pins_ = pins;
    } else {
      getPinsSet();
    }
  }

  PinManager(const PinManager& pin_manager) = delete;
  PinManager& operator=(const PinManager& pin_manager) = delete;

  /**
   *  @brief  Put key to pinstore directly.
   *
   *  @param cid key to pin.
   */
  void Pin(const common::Cid& cid) noexcept;
  /**
   *  @brief  Delete key in pinstore.
   *
   *  @param cid key to unpin.
   */
  void UnPin(const common::Cid& cid) noexcept;

 private:
  std::string getPins() noexcept;

  void getPinsSet() noexcept;

  void clearFile() noexcept;

  void addPins(const std::string& str_cid) noexcept;

  PinStore pinstore_;
  Pins pins_;

  common::logger::Logger logger_;
};

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio
#endif  // CNGT_REPO_PINNER_PIN_MANAGER_HPP_
