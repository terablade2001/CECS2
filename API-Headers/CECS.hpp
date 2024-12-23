#pragma once
// NOLINTBEGIN
#include <memory>
#include <string>
#include <utility>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cassert>
// NOLINTEND

class BaseCECS {
public:
  BaseCECS() = delete;
  BaseCECS(const BaseCECS&) = delete; // Prevent copy
  BaseCECS& operator=(const BaseCECS&) = delete; // Prevent assignment
  ~BaseCECS();

  static BaseCECS* getInstance(const std::string& ecsNameStr_);
  std::string getECSName() const;

private:
  explicit BaseCECS(std::string ecsNameStr_);
  static BaseCECS* instancePtr;
  std::string ecsName;
};

