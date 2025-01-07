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

#ifndef __FNAME__
  #define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
  #define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__ )
#endif
// NOLINTEND

class CECSSingleton {
public:
  CECSSingleton() = delete;
  CECSSingleton(const CECSSingleton&) = delete; // Prevent copy
  CECSSingleton& operator=(const CECSSingleton&) = delete; // Prevent assignment
  ~CECSSingleton();

  explicit CECSSingleton(std::string ecsNameStr_);

  static CECSSingleton& getInstance();
  std::string getECSName() const;
  void setECSName(const std::string &ecsName_);

private:
  static CECSSingleton instance;
  std::string ecsName;
};

