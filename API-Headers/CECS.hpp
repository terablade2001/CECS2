#pragma once
#include <string>

class CECS {
public:
  CECS() = delete;
  explicit CECS(const std::string& modNameStr_ = "", const std::string& ecsNameStr_ = "");
  CECS(const CECS& obj)  = delete; // No copy constructor.
  ~CECS();

  static CECS* getInstance();

private:
  static CECS* instancePtr;
  std::string ecsName;
  std::string modName;
};