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

bool CECSErrorCodes::isTagExistInMap(
    const std::string &tag_
) const noexcept {
  return mapTagsToErrorcodes.count(tag_) != 0;
}

// -------------------------------------------------------------------------------------------------




int CECSErrorCodesAtExit::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  const int Code = newErrorCode_.code;
  _ERRI(Code <= 1, "CECSErrorCodesAtExit: Id [=%i] of the new error code <= 1", Code)
  _ERRI(Code > 127, "CECSErrorCodesAtExit: Id [=%i] of the new error code > 127", Code)
  _ERRI(tag_.empty(), "CECSErrorCodesAtExit: Provided Tag_ is empty")
  _ERRI(
      newErrorCode_.description.empty(),
      "CECSErrorCodesAtExit: Provided new error description is empty"
  )
  _ERRI(
      isTagExistInMap(tag_), "CECSErrorCodesAtExit: Provided new error tag [=%s] already exist!",
      tag_.c_str()
  )
  for (const auto &el: mapTagsToErrorcodes) {
    _ERRI(el.second.code == Code, "CECSErrorCodesAtExit: Id [=%i] of the new error code already exist!",Code)
  }
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}

int CECSErrorCodesOnIntReturn::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  const int Code = newErrorCode_.code;
  _ERRI(
      (Code >= 0) && (Code <= 127),
      "CECSErrorCodesOnIntReturn: Id [=%i] of the new error code is in range [1,127]", Code
  )
  _ERRI(tag_.empty(), "CECSErrorCodesOnIntReturn: Provided Tag_ is empty")
  _ERRI(
      newErrorCode_.description.empty(),
      "CECSErrorCodesOnIntReturn: Provided new error description is empty"
  )
  _ERRI(
      isTagExistInMap(tag_), "CECSErrorCodesOnIntReturn: Provided new error tag [=%s] already exist!",
      tag_.c_str()
  )
  for (const auto &el: mapTagsToErrorcodes) {
    _ERRI(el.second.code == Code, "CECSErrorCodesOnIntReturn: Id [=%i] of the new error code already exist!",Code)
  }
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}
