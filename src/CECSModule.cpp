#include <CECSModule.hpp>
#include <cstdarg>

// NOLINTBEGIN
#define CECS__FERRORL (512)

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

void CECSModule::RecError(
    const char *fileName_, const uint32_t line_, const std::string &errId, const char *msg_, ...
) const noexcept(false) {
  if (fileName_ == nullptr) throw std::invalid_argument("CECS::RecError():: fileName_ is nullptr!");
  if (msg_ == nullptr) throw std::invalid_argument("CECS::RecError():: msg_ is nullptr!");
  char vaStr[CECS__FERRORL + 1] = {0};
  int  len                      = 0;
  va_list(vargs);
  va_start(vargs, msg_);
  len = vsnprintf(vaStr, CECS__FERRORL, msg_, vargs);
  va_end(vargs);
  if (len <= 0) snprintf(vaStr, CECS__FERRORL, "CECS::RecError():: %i = vsnprintf() >> failed!");
  ostringstream oss;
  oss << "(" << fileName_ << "), L-" << line_ << ": " << vaStr;
  CECS.critMsg(oss.str(), errId);
}

void CECSModule::RecError(
    const char *fileName_, const uint32_t line_, const std::string &errId, const std::string &msg_
) const noexcept(false) {
  if (fileName_ == nullptr) throw std::invalid_argument("CECS::RecError():: fileName_ is nullptr!");
  ostringstream oss;
  oss << "(" << fileName_ << "), L-" << line_ << ": " << msg_;
  CECS.critMsg(oss.str(), errId);
}
