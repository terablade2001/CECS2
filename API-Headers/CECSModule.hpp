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
  
};
