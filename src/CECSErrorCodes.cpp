#include <CECSErrorCodes.hpp>

CECS_MODULE(
    "CECSErrorCodes"
)

CECSErrorCodes::CECSErrorCodes() {
  mapTagsToErrorcodes.emplace("GENERIC", ErrorCodeList{1, "Generic Error."});
}

void CECSErrorCodes::reset() noexcept {
  mapTagsToErrorcodes.clear();
  mapTagsToErrorcodes.emplace("GENERIC", ErrorCodeList{1, "Generic Error."});
}

// -------------------------------------------------------------------------------------------------




int CECSErrorCodesAtExit::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  _ERRI(newErrorCode_.code <= 1, "...")
  return 0;
}

std::string CECSErrorCodesAtExit::getErrorCodesListing() const noexcept {
  std::ostringstream oss;
  for (const auto &el: mapTagsToErrorcodes) {
    oss << "\n - " << std::setw(4) << std::setfill(' ') << el.second.code << ": [" << el.first
        << "]" << el.second.description;
  }
  return oss.str();
}
