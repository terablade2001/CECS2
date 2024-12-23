#include <CECS.hpp>



BaseCECS* BaseCECS::instancePtr = nullptr;

BaseCECS::BaseCECS(std::string ecsNameStr_)
    : ecsName(std::move(ecsNameStr_)) { }

BaseCECS::~BaseCECS() {
  delete instancePtr;
}

BaseCECS *BaseCECS::getInstance(const std::string &ecsNameStr_) {
  static std::mutex instanceMutex; // Ensure thread safety
  std::lock_guard<std::mutex> lock(instanceMutex);

  if (instancePtr == nullptr) {
    instancePtr = new BaseCECS(ecsNameStr_);
  }
  return instancePtr;
}

std::string BaseCECS::getECSName() const {
  return ecsName;
}