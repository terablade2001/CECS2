#pragma once
#include <CECS.hpp>
#include <utility>

class CECSErrorCodes {
public:

  struct ErrorCodeList {
    int         code;
    std::string description;

    ErrorCodeList(
        const int code, std::string description
    ) : code(code), description(std::move(description)) {}
  };

  CECSErrorCodes();

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
