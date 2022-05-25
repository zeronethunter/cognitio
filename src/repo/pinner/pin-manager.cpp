#include "repo/pinner/pin-manager.hpp"

namespace cognitio {
namespace repo {
namespace pinner {

std::string PinManager::getPins() noexcept {
  std::fstream file(pinstore_.string(), std::ios::in);

  if (!file.is_open()) {
    logger_->error("Can not get pins, check if repo is opened.");
  }

  std::string result((std::istreambuf_iterator<char>(file)),
                     (std::istreambuf_iterator<char>()));

  file.close();

  return result;
}

void PinManager::clearFile() noexcept {
  std::fstream file(pinstore_.string(), std::ios::out | std::ios::trunc);

  if (!file.is_open()) {
    logger_->error("Can not clear pins, check if repo is opened.");
  }

  file.close();
}

void PinManager::addPins(const std::string &str_cid) noexcept {
  std::fstream file(pinstore_.string(), std::ios::out);

  if (!file.is_open()) {
    logger_->error("Can not add pins, check if repo is opened.");
  }

  file.write(str_cid.c_str(), static_cast<long>(str_cid.length()));

  file.close();
}

void PinManager::Pin(const common::Cid &cid) noexcept {
  std::string pins = getPins();
  pins += cid.ToString() + '\n';

  clearFile();

  addPins(pins);
}

void PinManager::UnPin(const common::Cid &cid) noexcept {
  std::string pins = getPins();

  std::string str_cid = cid.ToString();
  size_t pos = pins.find(str_cid);
  if (pos != std::string::npos) {
    pins.erase(pos, str_cid.length() + 1);
  }

  clearFile();

  addPins(pins);
}
void PinManager::getPinsSet() noexcept {
  std::string pins_str = getPins();

  size_t pos;

  while ((pos = pins_str.find('\n')) != std::string::npos) {
    pins_.insert(pins_str.substr(0, pos));
    pins_str.erase(0, pos + 1);
  }
}
}  // namespace pinner
}  // namespace repo
}  // namespace cognitio