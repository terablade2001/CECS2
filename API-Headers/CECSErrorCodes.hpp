#pragma once
#include <CECS.hpp>

class CECSErrorCodes {
public:
  constexpr std::string GenericErrorDescription{"Generic Error."};

  struct ErrorCodeList {
    int         code{0};
    std::string description{};
  } errorCodeList;

  CECSErrorCodes() : mapTagsToErrorcodes{{"GENERIC", {1, GenericErrorDescription}}} {}

  virtual ~CECSErrorCodes() = default;

  void reset() noexcept;

  virtual int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &&newErrorCode_) noexcept = 0;
  virtual std::string getErrorCodesListing() const noexcept                              = 0;

protected:
  std::map<std::string, ErrorCodeList> mapTagsToErrorcodes;
};

// -------------------------------------------------------------------------------------------------




class CECSErrorCodesAtExit final : public CECSErrorCodes {
public:
  CECSErrorCodesAtExit() = default;

  int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &&newErrorCode_) noexcept override;
  std::string getErrorCodesListing() const noexcept override;
};
