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

// NOLINTBEGIN
#ifndef __FNAME__
  #define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
  #define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__ )
#endif
// NOLINTEND

class BaseCECS {
public:
  BaseCECS() = delete;
  BaseCECS(const BaseCECS&) = delete; // Prevent copy
  BaseCECS& operator=(const BaseCECS&) = delete; // Prevent assignment
  ~BaseCECS();

  static BaseCECS* getInstance();
  std::string getECSName() const;

private:
  explicit BaseCECS(std::string ecsNameStr_);
  static BaseCECS* instancePtr;
  std::string ecsName;
};

