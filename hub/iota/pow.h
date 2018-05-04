// Copyright 2018 IOTA Foundation

#ifndef HUB_IOTA_POW_H_
#define HUB_IOTA_POW_H_

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace hub {
namespace iota {

class IotaAPI;
class POWProvider {
 public:
  POWProvider(std::shared_ptr<iota::IotaAPI> api, size_t depth, size_t mwm)
      : _api(std::move(api)), _depth(depth), _mwm(mwm) {}

  virtual ~POWProvider() {}

  size_t mwm() const { return _mwm; }
  size_t depth() const { return _depth; }

  virtual std::pair<std::string, std::string> getAttachmentPoint(
      const std::optional<std::string>& reference = {}) = 0;

  virtual std::vector<std::string> performPOW(
      const std::vector<std::string>& trytes) {
    auto location = getAttachmentPoint({});
    return doPOW(trytes, location.first, location.second);
  }

 protected:
  virtual std::vector<std::string> doPOW(const std::vector<std::string>& trytes,
                                         const std::string& trunk,
                                         const std::string& branch) = 0;

 protected:
  const std::shared_ptr<iota::IotaAPI> _api;
  const size_t _depth;
  const size_t _mwm;
};

class POWManager {
 public:
  static POWManager& get();

  POWProvider& provider() { return *_provider; }

  void setProvider(std::unique_ptr<POWProvider> provider) {
    _provider = std::move(provider);
  }

 private:
  std::unique_ptr<POWProvider> _provider;
};

}  // namespace iota
}  // namespace hub

#endif  // HUB_IOTA_POW_H_
