#include <CECS.hpp>


using namespace std;

CECSSingleton              CECSSingleton::instance{"CECS-Default"};
shared_ptr<spdlog::logger> CECSSingleton::logger{nullptr};

std::string CECSSingleton::CECSConfiguration::str() const {
  std::ostringstream os;
  os << "CECSConfiguration:\n"
     << " loggerName: " << loggerName << "\n screenLogLevel: " << static_cast<int>(screenLogLevel)
     << "\n fileLogLevel: " << static_cast<int>(fileLogLevel) << "\n logFileName: " << logFileName
     << "\n logFileMaxSizeBytes: " << logFileMaxSizeBytes
     << "\n logFileNumOfRotatingFiles: " << static_cast<int>(logFileNumOfRotatingFiles)
     << "\n useLogCustomFormat: " << useLogCustomFormat << "\n logCustomFormat: " << logCustomFormat
     << std::endl;
  return os.str();
}

CECSSingleton::CECSSingleton(
    std::string ecsNameStr_
) : ecsName(std::move(ecsNameStr_)) {
  setCECSConfiguration(defaultConfiguration);
}

void CECSSingleton::Shutdown() {
  logger = nullptr;
  state  = NOT_INIT;
}

CECSSingleton &CECSSingleton::getInstance() noexcept(
    false
) {
  return instance;
}

std::string CECSSingleton::getCECSName() const noexcept { return ecsName; }

void CECSSingleton::setCECSName(
    const std::string &ecsName_
) noexcept {
  static std::mutex           instanceMutex;
  std::lock_guard<std::mutex> lock(instanceMutex);
  ecsName = ecsName_;
}

void CECSSingleton::setCECSConfiguration(
    const CECSConfiguration &config
) noexcept(false) {
  if (state == INTERNAL_ERROR) {
    throw std::invalid_argument("State is in INTERNAL_ERROR. Can not proceed.");
  }
  if (config.loggerName.empty()) {
    state = State::INTERNAL_ERROR;
    throw std::invalid_argument("Logger name can not be empty!");
  }

  try {
    verifyEnumsHaveNotChange();
    std::vector<spdlog::sink_ptr> sinks;

    // Setup console sink --------------------------------------------------------------------------
    const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    switch (config.screenLogLevel) {
      case (5): {
        console_sink->set_level(spdlog::level::trace);
        break;
      }
      case (4): {
        console_sink->set_level(spdlog::level::debug);
        break;
      }
      case (3): {
        console_sink->set_level(spdlog::level::info);
        break;
      }
      case (2): {
        console_sink->set_level(spdlog::level::warn);
        break;
      }
      case (1): {
        console_sink->set_level(spdlog::level::err);
        break;
      }
      case (0): {
        console_sink->set_level(spdlog::level::critical);
        break;
      }
      default: {
        // NOLINTNEXTLINE
        const std::string err_msg = string("Console Log: Invalid screenlogLevel = ") +
                                    std::to_string(config.screenLogLevel);
        throw std::invalid_argument(err_msg);
      }
    }

    // Setup File Log sink -------------------------------------------------------------------------
    if (config.fileLogLevel < static_cast<uint8_t>(Logger::L::NONE)) {
      const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          config.logFileName.c_str(), config.logFileMaxSizeBytes, config.logFileNumOfRotatingFiles
      );
      switch (config.fileLogLevel) {
        case (static_cast<int>(Logger::L::TRC)): {
          file_sink->set_level(spdlog::level::trace);
          break;
        }
        case (static_cast<int>(Logger::L::DBG)): {
          file_sink->set_level(spdlog::level::debug);
          break;
        }
        case (static_cast<int>(Logger::L::INFO)): {
          file_sink->set_level(spdlog::level::info);
          break;
        }
        case (static_cast<int>(Logger::L::WARN)): {
          file_sink->set_level(spdlog::level::warn);
          break;
        }
        case (static_cast<int>(Logger::L::ERR)): {
          file_sink->set_level(spdlog::level::err);
          break;
        }
        case (static_cast<int>(Logger::L::CRIT)): {
          file_sink->set_level(spdlog::level::critical);
          break;
        }
        default: {
          // NOLINTNEXTLINE
          const std::string err_msg =
              string("Console Log: Invalid fileLogLevel = ") + std::to_string(config.fileLogLevel);
          throw std::invalid_argument(err_msg);
        }
      }
      sinks.push_back(file_sink);
    }

    logger = std::make_shared<spdlog::logger>(config.loggerName, sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::trace);
    if (config.useLogCustomFormat) {
      logger->set_pattern(config.logCustomFormat);
    }

  } catch (std::exception &) {
    state = State::INTERNAL_ERROR;
    throw;
  }

  state = INIT;
}

void CECSSingleton::reloadCECSConfiguration() noexcept(
    false
) {
  Shutdown();
  setCECSConfiguration(defaultConfiguration);
}

void CECSSingleton::logMsg(const Logger::L level_, const std::string &log_) const noexcept(false) {
  if (logger == nullptr) {
    throw std::runtime_error(
        "CECS - logMsg() Failed:: Logger has not initialized. Use setCECSConfiguration() ..."
    );
  }
  if (state != INIT) {
    throw std::runtime_error(
        "CECS - logMsg() Failed:: Logger is in wrong state. Shutdown and reconfigure."
    );
  }
  spdlog::level::level_enum spdLogLevel;
  switch (level_) {
    case Logger::CRIT:
      spdLogLevel = spdlog::level::critical;
      break;
    case Logger::ERR:
      spdLogLevel = spdlog::level::err;
      break;
    case Logger::WARN:
      spdLogLevel = spdlog::level::warn;
      break;
    case Logger::INFO:
      spdLogLevel = spdlog::level::info;
      break;
    case Logger::DBG:
      spdLogLevel = spdlog::level::debug;
      break;
    case Logger::TRC:
      spdLogLevel = spdlog::level::trace;
      break;
    default:
      throw std::invalid_argument("CECS - logMsg() Failed:: Invalid log level");
  }

  try {
    cout << "Logging: " << log_ << endl;
    logger->log(spdLogLevel, log_);
  } catch (std::exception &e) {
    string errMsg{
        "CECS - logMsg() Failed:: Logger has not initialized. Use setCECSConfiguration() ..."
    };
    errMsg += e.what();
    throw std::runtime_error(errMsg);
  }
}

void CECSSingleton::verifyEnumsHaveNotChange() noexcept(
    false
) {
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
