
#include <CECS.hpp>


using namespace std;

CECSSingleton              CECSSingleton::instance{"CECS-Default"};
shared_ptr<spdlog::logger> CECSSingleton::logger{nullptr};

std::string CECSConfiguration::str() const {
  std::ostringstream os;
  os << "CECSConfiguration:\n"
     << " loggerName: " << loggerName << "\n screenLogLevel: " << screenLogLevel
     << "\n fileLogLevel: " << fileLogLevel << "\n logFileName: " << logFileName
     << "\n logFileMaxSizeBytes: " << logFileMaxSizeBytes
     << "\n logFileNumOfRotatingFiles: " << logFileNumOfRotatingFiles
     << "\n useLogCustomFormat: " << useLogCustomFormat << "\n logCustomFormat: " << logCustomFormat
     << std::endl;
  return os.str();
}

CECSSingleton::CECSSingleton(
    std::string ecsNameStr_
) : ecsName(std::move(ecsNameStr_)) {
  setECSConfiguration(defaultConfiguration);
}

void CECSSingleton::Shutdown() {
  logger = nullptr;
  state  = NOT_INIT;
}

CECSSingleton::~CECSSingleton() { }

CECSSingleton &CECSSingleton::getInstance() { return instance; }

std::string CECSSingleton::getECSName() const noexcept { return ecsName; }

void CECSSingleton::setECSName(
    const std::string &ecsName_
) noexcept {
  static std::mutex           instanceMutex;
  std::lock_guard<std::mutex> lock(instanceMutex);
  ecsName = ecsName_;
}

void CECSSingleton::setECSConfiguration(
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
    if (config.fileLogLevel > 0) {
      const auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          config.logFileName.c_str(), config.logFileMaxSizeBytes, config.logFileNumOfRotatingFiles
      );
      switch (config.fileLogLevel) {
        case (5): {
          file_sink->set_level(spdlog::level::trace);
          break;
        }
        case (4): {
          file_sink->set_level(spdlog::level::debug);
          break;
        }
        case (3): {
          file_sink->set_level(spdlog::level::info);
          break;
        }
        case (2): {
          file_sink->set_level(spdlog::level::warn);
          break;
        }
        case (1): {
          file_sink->set_level(spdlog::level::err);
          break;
        }
        case (0): {
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

  } catch (std::exception &e) {
    state = State::INTERNAL_ERROR;
    throw e;
  }

  state = INIT;
}
