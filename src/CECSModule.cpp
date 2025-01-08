#include <CECSModule.hpp>
#include <cstdarg>

// NOLINTBEGIN
#define CECS__FERRORL (512)

// NOLINTEND

int CECSModule::getCompiledCECSMaxLineSize() { return CECS__FERRORL; }

void CECSModule::RecError(
    const char *fileName_, uint32_t line_, const std::string &errId, const char *msg_, ...
) const noexcept(false) {
  char vaStr[CECS__FERRORL + 1] = {0};
  int  len                      = 0;
  va_list(vargs);
  va_start(vargs, msg_);
  len = vsnprintf(vaStr, CECS__FERRORL, msg_, vargs);
  va_end(vargs);
  if (len <= 0) snprintf(vaStr, CECS__FERRORL, "CECS::RecError():: %i = vsnprintf() >> failed!");
  CECS.critMsg(vaStr, errId);
}

void CECSModule::RecError(
    const char *fileName_, uint32_t line_, const std::string &errId, const std::string &msg_
) const noexcept(false) {
  CECS.critMsg(msg_, errId);
}
