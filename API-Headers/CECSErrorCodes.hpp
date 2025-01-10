#pragma once
// NOLINTBEGIN
#include <memory>
#include <string>
#include <utility>
#include <cstdint>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cassert>
#include <map>
#include <ostream>
#include <CECSMacros.hpp>

//  NOLINTEND

class CECSErrorCodes {
public:
  struct ErrorCodeList {
    int         code{0};
    std::string description{};
  } errorCodeList;

  CECSErrorCodes() : mapTagsToErrorcodes{{"GENERIC", {1, GenericErrorDescription}}} {}

  virtual ~CECSErrorCodes() = default;

  void reset() noexcept;

  virtual int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &&newErrorCode_) noexcept;

private:
  constexpr std::string GenericErrorDescription{"Generic Error."};
  std::map<std::string, ErrorCodeList> mapTagsToErrorcodes;
};
