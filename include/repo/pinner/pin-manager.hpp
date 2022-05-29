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
#include "proto/ProtoPinner.pb.h"
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
  typedef std::filesystem::path Root;

 public:
  PinManager() = delete;

  explicit PinManager(Root root) noexcept
      : root_(std::move(root)),
        logger_(common::logger::createLogger("pinner")) {}

  PinManager(const PinManager& pin_manager) = delete;
  PinManager& operator=(const PinManager& pin_manager) = delete;

  PinManager& operator=(PinManager&& pin_manager) noexcept {
    pins_ = std::move(pin_manager.pins_);
    root_ = std::move(pin_manager.root_);
    return *this;
  }

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

  /**
   *  @brief  Check if pinstore exists.
   *
   *  @param cid key to unpin.
   */
  [[nodiscard]] bool Exists() const noexcept {
    return std::filesystem::exists(root_ / "Pins");
  }

  [[nodiscard]] Pins PinSet() noexcept {
    if (!get()) {
      logger_->warn("Failed to get pins.");
    }
    return pins_;
  }

  bool IsPinned(const common::Cid& cid) noexcept;

  void Dump() {
    if (Exists()) {
      logger_->info("Pins already exists at {}", (root_ / "Pins").string());
      get();
    } else {
      dump();
      logger_->info("Pins created {}", (root_ / "Pins").string());
    }
  }

 private:
  ssize_t getIndex(const common::Cid& cid) const noexcept;
  bool dump() const noexcept;
  bool get() noexcept;

  Root root_;
  Pins pins_ = Pins();

  common::logger::Logger logger_;
};

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio
#endif  // CNGT_REPO_PINNER_PIN_MANAGER_HPP_
