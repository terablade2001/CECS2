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
}

// -------------------------------------------------------------------------------------------------

class CECSSingleton {
public:
  enum State { NOT_INIT = 0, INIT = 1, INTERNAL_ERROR = 2 };

  enum ErrorMode { CRITICAL = 5, ERROR = 4 };

  std::atomic<State> state{State::NOT_INIT};

  struct Configuration {
    std::string loggerName{"CECS"};
    uint8_t     screenLogLevel{Logger::L::TRC};
    uint8_t     fileLogLevel{Logger::L::NONE};
    std::string logFileName{"CECSLog.log"};
    uint32_t    logFileMaxSizeBytes{100000};
    uint8_t     logFileNumOfRotatingFiles{3};
    std::string logCustomFormatForScreen{"[%^-%L-%$] %v"};
    std::string logCustomFormatForFile{"(%Y-%m-%d %H:%M:%S.%e) [%^-%L-%$] [t:%t] %v"};
    uint8_t     flushLevel{Logger::L::DBG};

    std::string str() const;
  };

  CECSSingleton()                                 = delete;
  CECSSingleton(const CECSSingleton &)            = delete;
  CECSSingleton &operator=(const CECSSingleton &) = delete;
  ~CECSSingleton()                                = default;

  static CECSSingleton &getInstance() noexcept(false);

  std::string getProjectName() const noexcept;
  void        setProjectName(const std::string &projectName_) noexcept;
  void        reconfigure() noexcept(false);
  void        logMsg(Logger::L level_, const std::string &log_) const noexcept(false);
  void        critMsg(const std::string &log_, const std::string &errId = "") noexcept(false);
  void        Shutdown();

  static uint32_t getNumberOfErrors() noexcept;
  static void
  resetNumberOfErrors(uint32_t reduceValue = std::numeric_limits<uint32_t>::max()) noexcept;

  static void resetNumberOfErrorsWithErrorModeCheck(
      uint32_t reduceValue = std::numeric_limits<uint32_t>::max()
  ) noexcept(false);

  static int       getDefaultErrorReturnValue() noexcept;
  static void      setErrorMode(ErrorMode mode_) noexcept(false);
  static ErrorMode getErrorMode() noexcept;
  Configuration    getConfiguration() noexcept;
  void             setConfiguration(Configuration config) noexcept;

  // Avoid using this method. Use reConfigure() instead.
  void initializeLogger(const Configuration &config) noexcept(false);

private:
  std::string                            projectName;
  Configuration                          configuration;
  static CECSSingleton                   instance;
  static std::atomic<ErrorMode>          errorMode;
  static std::recursive_mutex            cecsMtx;
  static std::atomic<uint32_t>           numberOfRecordedErrors;
  static std::shared_ptr<spdlog::logger> logger;

  explicit CECSSingleton(std::string ecsNameStr_);
  void        handleErrId(const std::string &errId) noexcept(false);
  static void verifyEnumsHaveNotChange() noexcept(false);
};
