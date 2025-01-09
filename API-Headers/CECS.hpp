#pragma once

// NOLINTBEGIN
#ifndef __cplusplus
static_assert(1, "C++ compiler (at least C++11) is required...");
#endif

#include <CECSModule.hpp>

#ifndef __FNAME__
#define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__)
#endif

#ifndef __ECSOBJ__
#define __ECSOBJ__ CECS_Instance_
#endif

#define CECS_MODULE(moduleName) static CECSModule __ECSOBJ__(moduleName);
#define CECS_MAIN_MODULE(moduleName, projetName) static CECS __ECSOBJ__(moduleName, projectName);


#define _ECSCLS_                                                                                   \
  { CECSSingleton::resetNumberOfErrors(); }
#define _ECSCLS(numberOfLatestRecords)                                                             \
  { CECSSingleton::resetNumberOfErrors(numberOfLatestRecords); }

#define _NERR_ (CECSSingleton::getNumberOfErrors())

#ifndef _MSC_VER

#define _ERRT(ExpR, args...)                                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    throw runtime_error("_ERRT occurred");                                                         \
  }
// #define _ERRTU(ExpR, UserErrID, args...)                                                           \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     throw runtime_error("_ERRT occurred");                                                         \
//   }

#define _ERRI(ExpR, args...)                                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    return CECSSingleton::getDefaultErrorReturnValue();                                            \
  }
// #define _ERRIU(ExpR, UserErrID, args...)                                                           \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     return CECSSingleton::getDefaultErrorReturnValue();                                            \
//   }

#define _ERR(ExpR, args...)                                                                        \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    return;                                                                                        \
  }
// #define _ERRU(ExpR, UserErrID, args...)                                                            \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     return;                                                                                        \
//   }

#define _ERRN(ExpR, args...)                                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    return nullptr;                                                                                \
  }
// #define _ERRNU(ExpR, UserErrID, args...)                                                           \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     return nullptr;                                                                                \
//   }

#define _ERRB(ExpR, args...)                                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    return false;                                                                                  \
  }
// #define _ERRBU(ExpR, UserErrID, args...)                                                           \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     return false;                                                                                  \
//   }

#define _ERRO(ExpR, __UserReturn__, args...)                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", args);                                            \
    __UserReturn__                                                                                 \
  }
// #define _ERROU(ExpR, __UserReturn__, UserErrID, args...)                                           \
//   if ((ExpR)) {                                                                                    \
//     __ECSOBJ__.RecError(__FNAME__, __LINE__, UserErrID, args);                                     \
//     __UserReturn__                                                                                 \
//   }

#define _ERRSTR(ExpR, __UserSS__)                                                                  \
  if ((ExpR)) {                                                                                    \
    std::ostringstream ss;                                                                         \
    __UserSS__;                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, "", ss.str());                                        \
  }

#define _CERRT_ _ERRT(0 != _NERR_, "_CERRT_: (%u) Error(/s) in CECS deteted.", _NERR_)
#define _CERRI_ _ERRI(0 != _NERR_, "_CERRI_: (%u) Error(/s) in CECS deteted.", _NERR_)
#define _CERR_ _ERR(0 != _NERR_, "_CERR_: (%u) Error(/s) in CECS deteted.", _NERR_)
#define _CERRN_ _ERRN(0 != _NERR_, "_CERRN_: (%u) Error(/s) in CECS deteted.", _NERR_)
#define _CERRB_ _ERRB(0 != _NERR_, "_CERRB_: (%u) Error(/s) in CECS deteted.", _NERR_)
#define _CERRO_(__UserReturn__)                                                                    \
  _ERRO(0 != _NERR_, __UserReturn__, "_CERRO_: (%u) Error(/s) in CECS deteted.", _NERR_)

#define _CERRT(args...) {_ERRT(0 != _NERR_, args)}
#define _CERR(args...) {_ERR(0 != _NERR_, args)}
#define _CERRI(args...) {_ERRI(0 != _NERR_, args)}
#define _CERRN(args...) {_ERRN(0 != _NERR_, args)}
#define _CERRB(args...) {_ERRB(0 != _NERR_, args)}
#define _CERRO(__UserReturn__, args...) {_ERRO(0 != _NERR_, __UserReturn__, args)}

#else
static_assert(1, "MSC compiler is not supported yet...");
#endif

// NOLINTEND
