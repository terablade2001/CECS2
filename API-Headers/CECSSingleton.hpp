#pragma once
// NOLINTBEGIN
#include <CECSMacros.hpp>
#include<atomic>
// NOLINTEND

namespace Logger {
  enum L { TRC = 0, DBG = 1, INFO = 2, WARN = 3, ERR = 4, CRIT = 5, NONE = 6 };
}

// -------------------------------------------------------------------------------------------------

class CECSErrorCodesAtExit;
class CECSErrorCodesOnIntReturn;
namespace spdlog { class logger; }

class CECSSingleton {
public:
  enum State { NOT_INIT = 0, INIT = 1, INTERNAL_ERROR = 2 };

  enum ErrorMode { CRITICAL_MODE = 5, ERROR_MODE = 4 };

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
    bool        isLoggingUsingModuleNameInsteadOfFilename{true};
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
  void
  resetNumberOfErrors(uint32_t reduceValue = std::numeric_limits<uint32_t>::max()) const noexcept;

  void resetNumberOfErrorsWithErrorModeCheck(
      uint32_t reduceValue = std::numeric_limits<uint32_t>::max()
  ) const noexcept(false);

  static int       getDefaultErrorReturnValue() noexcept;
  static void      setErrorMode(ErrorMode mode_) noexcept(false);
  static ErrorMode getErrorMode() noexcept;
  Configuration    getConfiguration() noexcept;
  void             setConfiguration(Configuration config) noexcept;
  int              getErrorIntegerAtExit() const noexcept(false);
  int              getErrorIntegerOnIntReturn() const noexcept(false);

  void setNewErrorAtExit(
      const std::string &tag_, int errorNum_, const std::string &description_
  ) const noexcept(false);
  void setNewErrorOnIntReturn(
      const std::string &tag_, int errorNum_, const std::string &description_
  ) const noexcept(false);

  std::string getErrorsMapAtExit() const noexcept(false);
  std::string getErrorsMapOnIntReturn() const noexcept(false);

  static std::string getCECSVersion() noexcept;
  static int         getCECSVersionMajor() noexcept;
  static int         getCECSVersionMinor() noexcept;
  static int         getCECSVersionPatch() noexcept;

  // Avoid using this method. Use reConfigure() instead.
  void initializeLogger(const Configuration &config) noexcept(false);

protected:
  friend class CECSModule;
  Configuration configuration;

private:
  std::string                            projectName;
  static CECSSingleton                   instance;
  static std::atomic<ErrorMode>          errorMode;
  static std::recursive_mutex            cecsMtx;
  static std::atomic<uint32_t>           numberOfRecordedErrors;
  static std::shared_ptr<spdlog::logger> logger;

  std::shared_ptr<CECSErrorCodesAtExit>      cecsErrorCodesAtExit;
  std::shared_ptr<CECSErrorCodesOnIntReturn> cecsErrorCodesOnIntReturn;

  explicit CECSSingleton(std::string ecsNameStr_);
  void        handleErrId(const std::string &errId) noexcept(false);
  bool        handleErrIdAtExit(const std::string &errId) const noexcept(false);
  bool        handleErrIdOnIntReturn(const std::string &errId) const noexcept(false);
  static void verifyEnumsHaveNotChange() noexcept(false);
};
