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
  static std::mutex instanceMutex; // Ensure thread safety
  std::lock_guard<std::mutex> lock(instanceMutex);
  return instance;
}

std::string CECSSingleton::getECSName() const {
  return ecsName;
}