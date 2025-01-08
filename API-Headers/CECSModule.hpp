#pragma once
#include <CECSSingleton.hpp>
#include <utility>

class CECSModule {
  std::string    moduleName;
  CECSSingleton &CECS;

public:
  CECSModule()                              = delete;
  CECSModule(const CECSModule &)            = delete;
  CECSModule &operator=(const CECSModule &) = delete;
  ~CECSModule()                             = default;

  // NOLINTNEXTLINE
  CECSModule(
      std::string moduleName_
  ) : moduleName(std::move(moduleName_)), CECS(CECSSingleton::getInstance()) {}

  CECSModule(
      std::string moduleName_, const std::string &projectName_
  ) : moduleName(std::move(moduleName_)), CECS(CECSSingleton::getInstance()) {
    CECS.setProjectName(projectName_);
  }

  static int getCompiledCECSMaxLineSize();

  void RecError(
      const char        *fileName_, // __FNAME__ : The filename of the code called
      uint32_t           line_,     // __LINE__ : The line of the coded which was called
      const std::string &errId,     // Return Err Id String for the system at exit.
      const char        *msg_,      // String message with format descriptors like printf()
      ...
  ) const noexcept(false);

  void RecError(
      const char        *fileName_, // __FNAME__ : The filename of the code called
      uint32_t           line_,     // __LINE__ : The line of the coded which was called
      const std::string &errId,     // Return Err Id String for the system at exit.
      const std::string &msg_       // String for the error
  ) const noexcept(false);
};
