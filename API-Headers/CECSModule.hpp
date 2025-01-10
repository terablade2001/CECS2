#pragma once
#include <CECSSingleton.hpp>

class CECSModule {
  std::string          moduleName;
  std::recursive_mutex mtx;

public:
  CECSSingleton &CECS;

  CECSModule()                              = delete;
  CECSModule(const CECSModule &)            = delete;
  CECSModule &operator=(const CECSModule &) = delete;
  ~CECSModule()                             = default;

  // NOLINTNEXTLINE
  CECSModule(const std::string &moduleName_);
  CECSModule(const std::string &moduleName_, const std::string &projectName_);

  static int getCompiledCECSMaxLineSize();

  void RecLog(Logger::L level_, const char *msg_, ...) noexcept(false);
  void RecLog(Logger::L level_, const std::string &msg_) noexcept(false);
  void RecLog(uint32_t line_, Logger::L level_, const std::string &msg_) noexcept(false);

  void RecError(
      const char        *fileName_, // __FNAME__ : The filename of the code called
      uint32_t           line_,     // __LINE__ : The line of the coded which was called
      const std::string &errId,     // Return Error ID String for the system at exit.
      const char        *msg_,      // String message with format descriptors like printf()
      ...
  ) noexcept(false);

  void RecError(
      const char        *fileName_, // __FNAME__ : The filename of the code called
      uint32_t           line_,     // __LINE__ : The line of the coded which was called
      const std::string &errId,     // Return Error ID String for the system at exit.
      const std::string &msg_       // String for the error
  ) noexcept(false);
};
