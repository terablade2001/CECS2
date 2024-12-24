#include <CECS.hpp>



BaseCECS* BaseCECS::instancePtr = nullptr;

BaseCECS::BaseCECS(std::string ecsNameStr_)
    : ecsName(std::move(ecsNameStr_)) { }

BaseCECS::~BaseCECS() {
  delete instancePtr;
}

BaseCECS *BaseCECS::getInstance() {
  static std::mutex instanceMutex; // Ensure thread safety
  std::lock_guard<std::mutex> lock(instanceMutex);

  if (instancePtr == nullptr) {
    instancePtr = new BaseCECS("CECS-Default");
  }
  return instancePtr;
}

std::string BaseCECS::getECSName() const {
  return ecsName;
}