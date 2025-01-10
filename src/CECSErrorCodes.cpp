#include <CECSErrorCodes.hpp>

void CECSErrorCodes::reset() noexcept {
  mapTagsToErrorcodes.clear();
  mapTagsToErrorcodes.emplace("GENERIC", ErrorCodeList{1, GenericErrorDescription});

}

int CECSErrorCodes::addNewErrorCode(
    const std::string &tag_, const ErrorCodeList &&newErrorCode_
) noexcept {
  return 0;
}
