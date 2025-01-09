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
#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
// NOLINTEND

namespace Logger {
  enum L { TRC = 0, DBG = 1, INFO = 2, WARN = 3, ERR = 4, CRIT = 5, NONE = 6 };
};

// -------------------------------------------------------------------------------------------------

class CECSSingleton {
public:
  enum State { NOT_INIT=0, INIT=1, INTERNAL_ERROR=2 } state{NOT_INIT};

  struct Configuration {
    std::string loggerName{"IL"};
    uint8_t     screenLogLevel{Logger::L::TRC};
    uint8_t     fileLogLevel{Logger::L::NONE};
    std::string logFileName{"CECSLog.log"};
    uint32_t    logFileMaxSizeBytes{10000};
    uint8_t     logFileNumOfRotatingFiles{3};
    bool        useLogCustomFormat{true};
    std::string logCustomFormat{"(%m/%d %H:%M:%S) [%^%L%$] [t:%t] %v"};
    uint8_t     flushLevel{Logger::L::INFO};

    std::string str() const;
  } configuration;

  CECSSingleton()                                 = delete;
  CECSSingleton(const CECSSingleton &)            = delete; // Prevent copy
  CECSSingleton &operator=(const CECSSingleton &) = delete; // Prevent assignment
  ~CECSSingleton()                                = default;

  static CECSSingleton &getInstance() noexcept(false);
  void                  Shutdown();

  std::string getProjectName() const noexcept;
  void        setProjectName(const std::string &projectName_) noexcept;
  void        setConfiguration(const Configuration &config) noexcept(false);
  void        reconfigure() noexcept(false);
  void        logMsg(Logger::L level_, const std::string &log_) const noexcept(false);
  void        critMsg(const std::string &log_, const std::string &errId = "") const noexcept(false);

  static uint32_t getNumberOfErrors() noexcept;
  static void
  resetNumberOfErrors(uint32_t reduceValue = std::numeric_limits<uint32_t>::max()) noexcept;

private:
  explicit CECSSingleton(std::string ecsNameStr_);
  static CECSSingleton                   instance;
  std::string                            projectName;
  static std::atomic<uint32_t>           numberOfRecordedErrors;
  static std::shared_ptr<spdlog::logger> logger;
  static void                            verifyEnumsHaveNotChange() noexcept(false);
};
