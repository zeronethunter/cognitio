#include "repo/pinner/pin-manager.hpp"

namespace cognitio {
namespace repo {
namespace pinner {

void PinManager::Pin(const common::Cid &cid) noexcept {
  logger_->debug("Pinning: {}", cid.ToString());

  if (!get()) {
    logger_->error("Failed to get pins");
    return;
  }

  ssize_t index = getIndex(cid);

  if (index != -1) {
    logger_->info("Is already pinned: {}", cid.ToString());
    return;
  }

  pins_.mutable_cids()->Add(cid.ToString());

  dump() ? logger_->debug("Successfully pinned: {}", cid.ToString())
         : logger_->debug("Failed to pin: {}", cid.ToString());
}

ssize_t PinManager::getIndex(const common::Cid &cid) const noexcept {
  ssize_t index = 0;
  for (auto i = pins_.cids().cbegin(); i < pins_.cids().cend(); ++i) {
    if (*i == cid.ToString()) {
      return index;
    }
    ++index;
  }
  return -1;
}

void PinManager::UnPin(const common::Cid &cid) noexcept {
  logger_->debug("Unpinning: {}", cid.ToString());

  if (!get()) {
    logger_->error("Failed to get pins");
    return;
  }

  ssize_t index = getIndex(cid);

  if (index == -1) {
    logger_->info("Is already unpinned: {}", cid.ToString());
    return;
  }

  pins_.mutable_cids()->erase(pins_.mutable_cids()->cbegin() +
                              static_cast<long>(index));

  dump() ? logger_->debug("Successfully unpinned: {}", cid.ToString())
         : logger_->debug("Failed to unpin: {}", cid.ToString());
}

bool PinManager::IsPinned(const common::Cid &cid) noexcept {
  if (!get()) {
    logger_->error("Failed to get pins");
    return false;
  }

  auto is_find = std::find(pins_.mutable_cids()->begin(),
                           pins_.mutable_cids()->end(), cid.ToString());

  return !(is_find == pins_.mutable_cids()->end());
}

bool PinManager::dump() const noexcept {
  std::fstream pinner((root_ / "pins").string(),
                      std::ios::out | std::ios::trunc);

  if (!pinner) {
    logger_->error("Failed to open pinner.");
  }
  bool is_dumped = pins_.SerializeToOstream(&pinner);

  pinner.close();

  return is_dumped;
}
bool PinManager:: get() noexcept {
  std::fstream pinner((root_ / "pins").string(), std::ios::in);

  if (!pinner) {
    logger_->error("Failed to open pinner.");
  }

  bool is_got = pins_.ParseFromIstream(&pinner);

  pinner.close();

  return is_got;
}

}  // namespace pinner
}  // namespace repo
}  // namespace cognitio