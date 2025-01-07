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
#include <ostream>

#ifndef __FNAME__
#define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__)
#endif
// NOLINTEND

struct CECSConfiguration {
  std::string loggerName{"IL"};
  int         screenLogLevel{5};
  int         fileLogLevel{3};
  std::string logFileName{"CECSLog.log"};
  int         logFileMaxSizeBytes{10000};
  int         logFileNumOfRotatingFiles{3};
  bool        useLogCustomFormat{false};
  // TODO:: Update teh default Custom Format to something more desired
  std::string logCustomFormat{"[%H:%M:%S] [%^%L%$] [%t] %v"};

  std::string str() const;
};

class CECSSingleton {
public:
  enum State { INIT, INTERNAL_ERROR } state{INIT};

  CECSSingleton()                                 = delete;
  CECSSingleton(const CECSSingleton &)            = delete; // Prevent copy
  CECSSingleton &operator=(const CECSSingleton &) = delete; // Prevent assignment
  ~CECSSingleton();

  explicit CECSSingleton(std::string ecsNameStr_);

  static CECSSingleton &getInstance();
  std::string           getECSName() const noexcept;
  void                  setECSName(const std::string &ecsName_) noexcept;
  void                  setECSConfiguration(const CECSConfiguration &config) noexcept(false);

private:
  static CECSSingleton instance;
  std::string          ecsName;
  CECSConfiguration    defaultConfiguration;
};
