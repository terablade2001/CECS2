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

std::string CECSErrorCodes::getErrorCodesListing() const noexcept {
  std::ostringstream oss;
  for (const auto &el: mapTagsToErrorcodes) {
    oss << "\n - " << std::setw(4) << std::setfill(' ') << el.second.code << ": [" << el.first
        << "]" << el.second.description;
  }
  return oss.str();
}

// -------------------------------------------------------------------------------------------------




int CECSErrorCodesAtExit::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  _ERRI(
      newErrorCode_.code <= 1, "CECSErrorCodesAtExit: Id [=%i] of the new error code <= 1",
      newErrorCode_.code
  )
  _ERRI(
      newErrorCode_.code > 127, "CECSErrorCodesAtExit: Id [=%i] of the new error code > 127",
      newErrorCode_.code
  )
  _ERRI(tag_.empty(), "CECSErrorCodesAtExit: Provided Tag_ is empty")
  _ERRI(
      newErrorCode_.description.empty(),
      "CECSErrorCodesAtExit: Provided new error description is empty"
  )
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}

int CECSErrorCodesOnIntReturn::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  _ERRI(
      (newErrorCode_.code >= 0) && (newErrorCode_.code <= 127),
      "CECSErrorCodesOnIntReturn: Id [=%i] of the new error code is in range [1,127]",
      newErrorCode_.code
  )
  _ERRI(tag_.empty(), "CECSErrorCodesOnIntReturn: Provided Tag_ is empty")
  _ERRI(
      newErrorCode_.description.empty(),
      "CECSErrorCodesOnIntReturn: Provided new error description is empty"
  )
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}
