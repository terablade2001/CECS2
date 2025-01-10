#include <CECSModule.hpp>
#include <cstdarg>

// NOLINTBEGIN
constexpr int CECS__FERRORL = 512;
constexpr int CECS__FLOGL   = 512;

// NOLINTEND

using namespace std;

CECSModule::CECSModule(
    const std::string &moduleName_ // NOLINT
) : moduleName(moduleName_), CECS(CECSSingleton::getInstance()) {
  // NOLINTBEGIN
  // cout << "CECSModule::CECSModule(const std::string &moduleName_):: " << moduleName_ << endl;
  // cout << CECS.state << endl;
  // NOLINTEND
}

CECSModule::CECSModule(
    const std::string &moduleName_, const std::string &projectName_ // NOLINT
) : moduleName(moduleName_), CECS(CECSSingleton::getInstance()) {
  CECS.setProjectName(projectName_);
}

int CECSModule::getCompiledCECSMaxLineSize() { return CECS__FERRORL; }

void CECSModule::RecLog(
    const Logger::L level_,
    const char *msg_, ...
) noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(mtx);
  char                                  vaStr[CECS__FLOGL + 1] = {0};
  int                                   len                    = 0;
  va_list(vargs);
  va_start(vargs, msg_);
  len = vsnprintf(vaStr, CECS__FLOGL, msg_, vargs);
  va_end(vargs);
  // NOLINTNEXTLINE
  if (len <= 0) snprintf(vaStr, CECS__FLOGL, "CECS::RecLog():: %i = vsnprintf() >> failed!");
  try {
    ostringstream oss;
    oss << "[" << moduleName << "] " << vaStr;
    CECS.logMsg(level_, oss.str());
  } catch (std::exception &e) {
    if (CECS.configuration.isLoggingUsingModuleNameInsteadOfFilename) {
      RecError(moduleName.c_str(), __LINE__, "", "CECS::RecLog():: " + string(e.what()));
    } else {
      RecError(__FNAME__, __LINE__, "", "CECS::RecLog():: " + string(e.what()));
    }
    throw runtime_error("CECS::RecLog() failed.");
  }
}

void CECSModule::RecLog(Logger::L level_, const std::string &msg_)  noexcept(false){
  std::lock_guard<std::recursive_mutex> lock(mtx);
  try {
    ostringstream oss;
    oss << "[" << moduleName << "] " << msg_;
    CECS.logMsg(level_, oss.str());
  } catch (std::exception &e) {
    if (CECS.configuration.isLoggingUsingModuleNameInsteadOfFilename) {
      RecError(moduleName.c_str(), __LINE__, "", "CECS::RecLog():: " + string(e.what()));
    } else {
      RecError(__FNAME__, __LINE__, "", "CECS::RecLog():: " + string(e.what()));
    }
    throw runtime_error("CECS::RecLog() failed.");
  }
}

void CECSModule::RecError(
    const char *fileName_, const uint32_t line_, const std::string &errId, const char *msg_, ...
)  noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(mtx);
  if (fileName_ == nullptr) throw std::invalid_argument("CECS::RecError():: fileName_ is nullptr!");
  if (msg_ == nullptr) throw std::invalid_argument("CECS::RecError():: msg_ is nullptr!");
  char vaStr[CECS__FERRORL + 1] = {0};
  int  len                      = 0;
  va_list(vargs);
  va_start(vargs, msg_);
  len = vsnprintf(vaStr, CECS__FERRORL, msg_, vargs);
  va_end(vargs);
  // NOLINTNEXTLINE
  if (len <= 0) snprintf(vaStr, CECS__FERRORL, "CECS::RecError():: %i = vsnprintf() >> failed!");
  ostringstream oss;
  if (CECS.configuration.isLoggingUsingModuleNameInsteadOfFilename) {
    oss << "(" << moduleName << ", L-" << line_ << "): " << vaStr;
  } else {
    oss << "(" << fileName_ << ", L-" << line_ << "): " << vaStr;
  }
  CECS.critMsg(oss.str(), errId);
}

void CECSModule::RecError(
    const char *fileName_, const uint32_t line_, const std::string &errId, const std::string &msg_
)  noexcept(false) {
  std::lock_guard<std::recursive_mutex> lock(mtx);
  if (fileName_ == nullptr) throw std::invalid_argument("CECS::RecError():: fileName_ is nullptr!");
  ostringstream oss;
  if (CECS.configuration.isLoggingUsingModuleNameInsteadOfFilename) {
    oss << "(" << moduleName << ", L-" << line_ << "): " << msg_;
  } else {
    oss << "(" << fileName_ << ", L-" << line_ << "): " << msg_;
  }
  CECS.critMsg(oss.str(), errId);
}
