#include <CECSErrorCodes.hpp>

CECS_MODULE(
    "CECSErrorCodes"
)

void CECSErrorCodes::addPreconfiguredErrorCodes() noexcept {
  mapTagsToErrorcodes.emplace("GENERIC", ErrorCodeList{1, "Generic Error."});
  mapTagsToErrorcodes.emplace(
      "UNLISTED_ERROR",
      ErrorCodeList{_CECS_MAGIC_THROW_ERRORCODE_, "Reserved error code for unlisted errors."}
  );
}

CECSErrorCodes::CECSErrorCodes() : errorCode{0} {}

void CECSErrorCodes::clearErrorCode() noexcept { errorCode = 0; }

std::string CECSErrorCodes::getErrorCodesListing() const noexcept {
  std::ostringstream oss;
  // for (const auto &el: mapTagsToErrorcodes) {
  for (auto it = mapTagsToErrorcodes.begin(); it != mapTagsToErrorcodes.end(); ++it) {
    const auto &el = *it;
    oss << " - " << std::setw(4) << std::setfill(' ') << el.second.code << ": [" << el.first
        << "] > " << el.second.description;
    if (std::next(it) != mapTagsToErrorcodes.end()) { oss << "\n"; }
  }
  return oss.str();
}

bool CECSErrorCodes::isTagExistInMap(
    const std::string &tag_
) const noexcept {
  return mapTagsToErrorcodes.count(tag_) != 0;
}

void  CECSErrorCodes::handleErrorCode(const std::string &tag_) noexcept(false){
  // Update the errorCode only the first time an error occurs. This error code is the reason for
  // failure
  if (errorCode == 0) {
    if (!isTagExistInMap(tag_)) {
      errorCode = _CECS_MAGIC_THROW_ERRORCODE_;
      throw std::runtime_error(
          "CECSErrorCodesAtExit::handleErrorCode(): Given tag_ [" + tag_ + "] should exist!"
      );
    }
    const auto &el = mapTagsToErrorcodes.at(tag_);
    errorCode      = el.code;
  }
}

// -------------------------------------------------------------------------------------------------



CECSErrorCodesAtExit::CECSErrorCodesAtExit() { addPreconfiguredErrorCodes(); }

int CECSErrorCodesAtExit::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &newErrorCode_
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
    _ERRI(
        el.second.code == Code,
        "CECSErrorCodesAtExit: Id [=%i] of the new error code already exist!", Code
    )
  }
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}

void CECSErrorCodesAtExit::reset() noexcept {
  mapTagsToErrorcodes.clear();
  addPreconfiguredErrorCodes();
  errorCode = 0;
}

// -------------------------------------------------------------------------------------------------


int CECSErrorCodesOnIntReturn::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &newErrorCode_
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
      isTagExistInMap(tag_),
      "CECSErrorCodesOnIntReturn: Provided new error tag [=%s] already exist!", tag_.c_str()
  )
  for (const auto &el: mapTagsToErrorcodes) {
    _ERRI(
        el.second.code == Code,
        "CECSErrorCodesOnIntReturn: Id [=%i] of the new error code already exist!", Code
    )
  }
  mapTagsToErrorcodes.emplace(tag_, newErrorCode_);
  return 0;
}

void CECSErrorCodesOnIntReturn::reset() noexcept {
  mapTagsToErrorcodes.clear();
  errorCode = 0;
}
