#include "CECSErrorCodes.hpp"

#include <atomic>

using namespace std;

static constexpr int CECS_DEFAULT_ERROR_RETURN_VALUE = std::numeric_limits<int>::min();

CECSSingleton                    CECSSingleton::instance{"CECS-Default"};
atomic<CECSSingleton::ErrorMode> CECSSingleton::errorMode{ErrorMode::CRITICAL};
std::recursive_mutex             CECSSingleton::cecsMtx;
atomic<uint32_t>                 CECSSingleton::numberOfRecordedErrors{0};
shared_ptr<spdlog::logger>       CECSSingleton::logger{nullptr};

std::string CECSSingleton::Configuration::str() const {
  std::ostringstream os;
  os << "Configuration:\n"
     << " loggerName: " << loggerName << "\n screenLogLevel: " << static_cast<int>(screenLogLevel)
     << "\n fileLogLevel: " << static_cast<int>(fileLogLevel) << "\n logFileName: " << logFileName
     << "\n logFileMaxSizeBytes: " << logFileMaxSizeBytes
     << "\n logFileNumOfRotatingFiles: " << static_cast<int>(logFileNumOfRotatingFiles)
     << "\n logCustomFormatForScreen: " << logCustomFormatForScreen
     << "\n logCustomFormatForFile: " << logCustomFormatForFile
     << "\n flushLevel: " << static_cast<int>(flushLevel) << std::endl;
  return os.str();
}

CECSSingleton::CECSSingleton(
    std::string ecsNameStr_
) : projectName(std::move(ecsNameStr_)) {
  cecsErrorCodesAtExit      = make_shared<CECSErrorCodesAtExit>();
  cecsErrorCodesOnIntReturn = make_shared<CECSErrorCodesOnIntReturn>();
  initializeLogger(configuration);
}

void CECSSingleton::Shutdown() {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesAtExit) { cecsErrorCodesAtExit->reset(); }
  if (cecsErrorCodesOnIntReturn) { cecsErrorCodesOnIntReturn->reset(); }
  logger                 = nullptr;
  numberOfRecordedErrors = 0;
  state                  = NOT_INIT;
}

CECSSingleton &CECSSingleton::getInstance() noexcept(
    false
) {
  return instance;
}

std::string CECSSingleton::getProjectName() const noexcept { return projectName; }

void CECSSingleton::setProjectName(
    const std::string &projectName_
) noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  projectName = projectName_;
}

void CECSSingleton::initializeLogger(
    const Configuration &config
) noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (state != NOT_INIT) {
    throw std::runtime_error("Unexpected state != NOT_INIT. Can not proceed.");
  }
  if (config.loggerName.empty()) {
    state = INTERNAL_ERROR;
    throw std::invalid_argument("Logger name can not be empty!");
  }
  if (logger != nullptr) {
    state = INTERNAL_ERROR;
    throw std::runtime_error("Error: initializeLogger() without Shutdown be called first.");
  }

  try {
    verifyEnumsHaveNotChange();
    std::vector<spdlog::sink_ptr> sinks;
    sinks.clear();

    // Setup console sink --------------------------------------------------------------------------
    if (static_cast<uint8_t>(config.screenLogLevel) < static_cast<uint8_t>(Logger::L::NONE)) {
      const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      console_sink->set_level(static_cast<spdlog::level::level_enum>(config.screenLogLevel));
      if (!config.logCustomFormatForScreen.empty()) {
        console_sink->set_pattern(config.logCustomFormatForScreen);
      }
      sinks.push_back(console_sink);
    }

    // Setup File Log sink -------------------------------------------------------------------------
    if (config.fileLogLevel < static_cast<uint8_t>(Logger::L::NONE)) {
      const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          config.logFileName.c_str(), config.logFileMaxSizeBytes, config.logFileNumOfRotatingFiles
      );
      file_sink->set_level(static_cast<spdlog::level::level_enum>(config.fileLogLevel));
      if (!config.logCustomFormatForFile.empty()) {
        file_sink->set_pattern(config.logCustomFormatForFile);
      }
      sinks.push_back(file_sink);
    }

    logger = std::make_shared<spdlog::logger>(config.loggerName, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    spdlog::flush_on(static_cast<spdlog::level::level_enum>(config.flushLevel));

  } catch (std::exception &) {
    state = INTERNAL_ERROR;
    throw;
  }

  state = INIT;
}

void CECSSingleton::reconfigure() noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  Shutdown();
  initializeLogger(configuration);
}

void CECSSingleton::logMsg(const Logger::L level_, const std::string &log_) const noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (logger == nullptr) {
    throw std::runtime_error(
        "CECS - logMsg() Failed:: Logger is not initialized. Use initializeLogger() ..."
    );
  }
  if (state != INIT) {
    throw std::runtime_error(
        "CECS - logMsg() Failed:: Logger is in wrong state. Shutdown and reconfigure."
    );
  }
  if (static_cast<uint8_t>(level_) < static_cast<uint8_t>(Logger::L::NONE)) {
    if (level_ == Logger::L::CRIT) {
      throw std::invalid_argument(
          "CECS - logMsg() Failed:: Invalid log level. For 'critical' messages use critMsg()."
      );
    }
    try {
      logger->log(static_cast<spdlog::level::level_enum>(level_), log_);
    } catch (std::exception &e) {
      string errMsg{"CECS - logMsg() Failed:: Logger is not initialized. Use initializeLogger() ..."
      };
      errMsg += e.what();
      throw std::runtime_error(errMsg);
    }
  }
}

void CECSSingleton::critMsg(const std::string &log_, const std::string &errId)  noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (logger == nullptr) {
    throw std::runtime_error(
        "CECS - critMsg() Failed:: Logger is not initialized. Use initializeLogger() ..."
    );
  }
  if (state != INIT) {
    throw std::runtime_error(
        "CECS - critMsg() Failed:: Logger is in wrong state. Shutdown and reconfigure."
    );
  }

  ++numberOfRecordedErrors;

  handleErrId(errId);

  try {
    const int _errorMode = static_cast<int>(errorMode);
    logger->log(static_cast<spdlog::level::level_enum>(_errorMode), log_);
  } catch (std::exception &e) {
    string errMsg{"CECS - critMsg() Failed:: Logger is not initialized. Use initializeLogger() ..."
    };
    errMsg += e.what();
    throw std::runtime_error(errMsg);
  }
}

uint32_t CECSSingleton::getNumberOfErrors() noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  return numberOfRecordedErrors;
}

void CECSSingleton::resetNumberOfErrors(
    const uint32_t reduceValue
) const noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (reduceValue >= numberOfRecordedErrors) {
    numberOfRecordedErrors = 0;
    if (cecsErrorCodesAtExit) { cecsErrorCodesAtExit->clearErrorCode(); }
    if (cecsErrorCodesOnIntReturn) { cecsErrorCodesOnIntReturn->clearErrorCode(); }
    return;
  }
  numberOfRecordedErrors -= reduceValue;
}

void CECSSingleton::resetNumberOfErrorsWithErrorModeCheck(uint32_t reduceValue) const noexcept(false){
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (errorMode.load() == ErrorMode::CRITICAL) {
    throw std::runtime_error(
        "CECS: Modifying Number of Errors in CRITICAL mode is prohibited! Use ERROR mode instead."
    );
  }
  resetNumberOfErrors(reduceValue);
}

int CECSSingleton::getDefaultErrorReturnValue() noexcept { return CECS_DEFAULT_ERROR_RETURN_VALUE; }

void CECSSingleton::setErrorMode(
    const ErrorMode mode_
) noexcept(false) {
  lock_guard<recursive_mutex> lock(cecsMtx);
  if (errorMode == mode_) return;
  if (logger != nullptr) {
    string outString{"CECS: *** ErrorMode Set to [ "};
    if (mode_ == ErrorMode::CRITICAL) {
      outString += "CRITICAL ] ***";
    } else {
      outString += "ERROR ] ***";
    }
    logger->log(spdlog::level::debug, outString);
  } else {
    throw std::runtime_error("CECS - setErrorMode() Failed:: Logger is not initialized.");
  }
  errorMode = mode_;
}

CECSSingleton::ErrorMode CECSSingleton::getErrorMode() noexcept { return errorMode; }

CECSSingleton::Configuration CECSSingleton::getConfiguration() noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  return configuration;
}

void CECSSingleton::setConfiguration(
    Configuration config
) noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  configuration = std::move(config);
}

int CECSSingleton::getErrorIntegerAtExit() const noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesAtExit == nullptr) {
    throw runtime_error("CECS: getErrorIntegerAtExit() failed. cecsErrorCodesAtExit is nullptr.");
  }
  return cecsErrorCodesAtExit->errorCode;
}

int CECSSingleton::getErrorIntegerOnIntReturn() const noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesOnIntReturn == nullptr) {
    throw runtime_error(
        "CECS: getErrorIntegerOnIntReturn() failed. cecsErrorCodesOnIntReturn is nullptr."
    );
  }
  return cecsErrorCodesOnIntReturn->errorCode;
}

void CECSSingleton::setNewErrorAtExit(
    const std::string &tag_, const int errorNum_, const std::string &description_
) const noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesAtExit == nullptr) {
    throw runtime_error("CECS: setNewErrorAtExit() failed. cecsErrorCodesAtExit is nullptr.");
  }
  const CECSErrorCodes::ErrorCodeList errorCodeList{errorNum_, description_};
  cecsErrorCodesAtExit->addNewErrorCode(tag_, errorCodeList);
}

void CECSSingleton::setNewErrorOnIntReturn(
  const std::string &tag_, int errorNum_, const std::string &description_
  ) const noexcept(false){
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesOnIntReturn == nullptr) {
    throw runtime_error(
        "CECS: setNewErrorOnIntReturn() failed. cecsErrorCodesOnIntReturn is nullptr."
    );
  }
  const CECSErrorCodes::ErrorCodeList errorCodeList{errorNum_, description_};
  cecsErrorCodesOnIntReturn->addNewErrorCode(tag_, errorCodeList);
}

std::string CECSSingleton::getErrorsMapAtExit() const noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesAtExit == nullptr) {
    throw runtime_error("CECS: getErrorsMapAtExit() failed. cecsErrorCodesAtExit is nullptr.");
  }
  return cecsErrorCodesAtExit->getErrorCodesListing();
}

std::string CECSSingleton::getErrorsMapOnIntReturn() const noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (cecsErrorCodesOnIntReturn == nullptr) {
    throw runtime_error(
        "CECS: getErrorsMapOnIntReturn() failed. cecsErrorCodesOnIntReturn is nullptr."
    );
  }
  return cecsErrorCodesOnIntReturn->getErrorCodesListing();
}

void CECSSingleton::verifyEnumsHaveNotChange() noexcept(
    false
) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  // NOLINTBEGIN
  bool   isEnumsMatches = true;
  string errorMsg;
  if (static_cast<int8_t>(Logger::L::TRC) != static_cast<int8_t>(spdlog::level::trace)) {
    errorMsg       = "Logger::L::TRC != ::level::trace";
    isEnumsMatches = false;
  }
  if (static_cast<int8_t>(Logger::L::DBG) != static_cast<int8_t>(spdlog::level::debug)) {
    errorMsg       = "Logger::L::DBG != ::level::debug";
    isEnumsMatches = false;
  }
  if (static_cast<int8_t>(Logger::L::INFO) != static_cast<int8_t>(spdlog::level::info)) {
    errorMsg       = "Logger::L::INFO != ::level::info";
    isEnumsMatches = false;
  }
  if (static_cast<int8_t>(Logger::L::WARN) != static_cast<int8_t>(spdlog::level::warn)) {
    errorMsg       = "Logger::L::WARN != ::level::warn";
    isEnumsMatches = false;
  }
  if (static_cast<int8_t>(Logger::L::ERR) != static_cast<int8_t>(spdlog::level::err)) {
    errorMsg       = "Logger::L::ERR != ::level::err";
    isEnumsMatches = false;
  }

  if (!isEnumsMatches) { throw std::invalid_argument(errorMsg); }
  // NOLINTEND
}

// NOLINTNEXTLINE
void CECSSingleton::handleErrId(const std::string &errId)  noexcept(false) {
  if (errId.empty()) return;
  if (errId == "__ERRSTR_CALL__") {
    --numberOfRecordedErrors;
    return;
  }

  const bool handled = handleErrIdOnIntReturn(errId);
  if (!handled) {
    handleErrIdAtExit(errId); // NOLINT
  }
}

bool  CECSSingleton::handleErrIdAtExit(const std::string &errId) const noexcept(false){
  if (cecsErrorCodesAtExit == nullptr) {
    throw runtime_error("CECS: handleErrIdAtExit() failed. cecsErrorCodesAtExit is nullptr.");
  }
  cecsErrorCodesAtExit->handleErrorCode(errId);
  return true;
}

bool CECSSingleton::handleErrIdOnIntReturn(const std::string &errId) const  noexcept(false){
  if (cecsErrorCodesOnIntReturn == nullptr) {
    throw runtime_error(
        "CECS: handleErrIdOnIntReturn() failed. cecsErrorCodesOnIntReturn is nullptr."
    );
  }
  const bool isTagExistOnIntReturn = cecsErrorCodesOnIntReturn->isTagExistInMap(errId);
  if (!isTagExistOnIntReturn) { return false; }
  cecsErrorCodesOnIntReturn->handleErrorCode(errId);
  return true;
}

// -------------------------------------------------------------------------------------------------
