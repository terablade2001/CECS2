#include <CECS.hpp>

CECSSingleton CECSSingleton::instance{"CECS-Default"};

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

CECSSingleton::~CECSSingleton() {
  // Clean Logger.
}

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
  if (config.loggerName.empty()) {
    state = State::INTERNAL_ERROR;
    throw std::invalid_argument("Logger name can not be empty!");
  }
  // TODO :: Set the ECS Configuration
}
