#include <CECS.hpp>



CECSSingleton CECSSingleton::instance{"CECS-Default"};

CECSSingleton::CECSSingleton(std::string ecsNameStr_)
    : ecsName(std::move(ecsNameStr_)) {
  // Initialize Logger.
}

CECSSingleton::~CECSSingleton() {
  // Clean Logger.
}

CECSSingleton& CECSSingleton::getInstance() {
  return instance;
}

std::string CECSSingleton::getECSName() const { return ecsName; }

void CECSSingleton::setECSName(
    const std::string &ecsName_
) {
  static std::mutex instanceMutex;
  std::lock_guard<std::mutex> lock(instanceMutex);
  ecsName = ecsName_;
}
