#pragma once
// NOLINTBEGIN
#include <memory>
#include <string>
#include <utility>
#include <cstdint>
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
#include <spdlog/common.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#ifndef __FNAME__
#define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__)
#endif
// NOLINTEND
// -------------------------------------------------------------------------------------------------

namespace Logger {
  enum L { CRIT = 5, ERR = 4, WARN = 3, INFO = 2, DBG = 1, TRC = 0 };
};

struct CECSConfiguration {
  std::string loggerName{"IL"};
  uint8_t     screenLogLevel{5};
  uint8_t     fileLogLevel{1};
  std::string logFileName{"CECSLog.log"};
  uint32_t    logFileMaxSizeBytes{10000};
  uint8_t     logFileNumOfRotatingFiles{3};
  bool        useLogCustomFormat{false};
  // TODO:: Update teh default Custom Format to something more desired
  std::string logCustomFormat{"[%H:%M:%S] [%^%L%$] [%t] %v"};

  std::string str() const;
};

// -------------------------------------------------------------------------------------------------




class CECSSingleton {
public:
  enum State { NOT_INIT, INIT, INTERNAL_ERROR } state{NOT_INIT};

  CECSConfiguration defaultConfiguration;

  CECSSingleton()                                 = delete;
  CECSSingleton(const CECSSingleton &)            = delete; // Prevent copy
  CECSSingleton &operator=(const CECSSingleton &) = delete; // Prevent assignment
  ~CECSSingleton()                                = default;

  static CECSSingleton &getInstance() noexcept(false);
  void                  Shutdown();

  std::string getECSName() const noexcept;
  void        setECSName(const std::string &ecsName_) noexcept;
  void        setECSConfiguration(const CECSConfiguration &config) noexcept(false);
  void        logMsg(Logger::L level_, const std::string &log_) const noexcept(false);

private:
  explicit CECSSingleton(std::string ecsNameStr_);
  static CECSSingleton                   instance;
  std::string                            ecsName;
  static std::shared_ptr<spdlog::logger> logger;
  static void                            verifyEnumsHaveNotChange() noexcept(false);
};
