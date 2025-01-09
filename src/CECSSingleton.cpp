#include <CECSSingleton.hpp>

using namespace std;

CECSSingleton              CECSSingleton::instance{"CECS-Default"};
std::recursive_mutex       CECSSingleton::cecsMtx;
atomic<uint32_t>           CECSSingleton::numberOfRecordedErrors{0};
shared_ptr<spdlog::logger> CECSSingleton::logger{nullptr};

static constexpr int CECS_DEFAULT_ERROR_RETURN_VALUE = std::numeric_limits<int>::min();

std::string CECSSingleton::Configuration::str() const {
  std::ostringstream os;
  os << "Configuration:\n"
     << " loggerName: " << loggerName << "\n screenLogLevel: " << static_cast<int>(screenLogLevel)
     << "\n fileLogLevel: " << static_cast<int>(fileLogLevel) << "\n logFileName: " << logFileName
     << "\n logFileMaxSizeBytes: " << logFileMaxSizeBytes
     << "\n logFileNumOfRotatingFiles: " << static_cast<int>(logFileNumOfRotatingFiles)
     << "\n useLogCustomFormat: " << useLogCustomFormat << "\n logCustomFormat: " << logCustomFormat
     << "\n flushLevel: " << static_cast<int>(flushLevel) << std::endl;
  return os.str();
}

CECSSingleton::CECSSingleton(
    std::string ecsNameStr_
) : projectName(std::move(ecsNameStr_)) {
  // NOLINTBEGIN
  // cout << "CECSSingleton : Setting configuration ..." << state << endl;
  setConfiguration(configuration);
  // cout << "CECSSingleton : Setting configuration DONE ..." << state << endl;
  // if (logger == nullptr) {
  //   cout << "*** CECSSingleton : Logger is nullptr! *** " << endl;
  // } else {
  //   cout << "CECSSingleton : Logger initialized! " << endl;
  // }
  // NOLINTEND
}

void CECSSingleton::Shutdown() {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  logger = nullptr;
  numberOfRecordedErrors = 0;
  state  = NOT_INIT;
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

void CECSSingleton::setConfiguration(
    const Configuration &config
) noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (state == INTERNAL_ERROR) {
    throw std::invalid_argument("State is in INTERNAL_ERROR. Can not proceed.");
  }
  if (config.loggerName.empty()) {
    state = INTERNAL_ERROR;
    throw std::invalid_argument("Logger name can not be empty!");
  }

  try {
    verifyEnumsHaveNotChange();
    std::vector<spdlog::sink_ptr> sinks;
    sinks.clear();

    // Setup console sink --------------------------------------------------------------------------
    if (static_cast<uint8_t>(config.screenLogLevel) < static_cast<uint8_t>(Logger::L::NONE)) {
      const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      console_sink->set_level(static_cast<spdlog::level::level_enum>(config.screenLogLevel));
      sinks.push_back(console_sink);
    }

    // Setup File Log sink -------------------------------------------------------------------------
    if (config.fileLogLevel < static_cast<uint8_t>(Logger::L::NONE)) {
      const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          config.logFileName.c_str(), config.logFileMaxSizeBytes, config.logFileNumOfRotatingFiles
      );
      file_sink->set_level(static_cast<spdlog::level::level_enum>(config.fileLogLevel));
      sinks.push_back(file_sink);
    }

    logger = std::make_shared<spdlog::logger>(config.loggerName, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    if (config.useLogCustomFormat) { logger->set_pattern(config.logCustomFormat); }
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
  setConfiguration(configuration);
}

void CECSSingleton::logMsg(const Logger::L level_, const std::string &log_) const noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (logger == nullptr) {
    throw std::runtime_error(
        "CECS - logMsg() Failed:: Logger has not initialized. Use setConfiguration() ..."
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
      string errMsg{
          "CECS - logMsg() Failed:: Logger has not initialized. Use setConfiguration() ..."
      };
      errMsg += e.what();
      throw std::runtime_error(errMsg);
    }
  }
}

void CECSSingleton::critMsg(const std::string &log_, const std::string &errId) const noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (logger == nullptr) {
    throw std::runtime_error(
        "CECS - critMsg() Failed:: Logger has not initialized. Use setConfiguration() ..."
    );
  }
  if (state != INIT) {
    throw std::runtime_error(
        "CECS - critMsg() Failed:: Logger is in wrong state. Shutdown and reconfigure."
    );
  }

  ++numberOfRecordedErrors;

  if (!errId.empty()) {
    // TODO : Handle the errId via proper mechanism.
  }

  try {
    logger->log(spdlog::level::critical, log_);
  } catch (std::exception &e) {
    string errMsg{"CECS - critMsg() Failed:: Logger has not initialized. Use setConfiguration() ..."
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
) noexcept {
  std::lock_guard<std::recursive_mutex> lock(cecsMtx);
  if (reduceValue >= numberOfRecordedErrors) {
    numberOfRecordedErrors = 0;
    return;
  }
  numberOfRecordedErrors -= reduceValue;
}

int CECSSingleton::getDefaultErrorReturnValue() noexcept {
  return CECS_DEFAULT_ERROR_RETURN_VALUE;
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

// -------------------------------------------------------------------------------------------------
