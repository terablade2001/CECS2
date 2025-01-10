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

  int errorCode;

  CECSErrorCodes();

  virtual ~CECSErrorCodes() = default;

  void        clearErrorCode() noexcept;
  std::string getErrorCodesListing() const noexcept;
  bool        isTagExistInMap(const std::string &tag_) const noexcept;
  int         handleErrorCode(const std::string &tag_) noexcept(false);


  virtual int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &newErrorCode_) noexcept = 0;
  virtual void reset() noexcept                                                         = 0;

protected:
  std::map<std::string, ErrorCodeList> mapTagsToErrorcodes;

  void addPreconfiguredErrorCodes() noexcept;
};

// -------------------------------------------------------------------------------------------------




class CECSErrorCodesAtExit final : public CECSErrorCodes {
public:
  CECSErrorCodesAtExit();

  int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &newErrorCode_) noexcept override;
  void reset() noexcept override;
};

class CECSErrorCodesOnIntReturn final : public CECSErrorCodes {
public:
  CECSErrorCodesOnIntReturn() = default;

  int
  addNewErrorCode(const std::string &tag_, const ErrorCodeList &newErrorCode_) noexcept override;
  void reset() noexcept override;
};
