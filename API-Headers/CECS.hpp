#pragma once

// NOLINTBEGIN
#ifndef __cplusplus
static_assert(1, "C++ compiler (at least C++11) is required...");
#endif

// ================================================================================================
// TODO:: Confirm at this section that no MACRO is already used (Example with _ERRT)
#ifdef _ERRT
static_assert(1, "_ERRT macro is already defined...");
#endif
// ================================================================================================


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

/*
#define _ERRO_ORG(Obj, ExpR, __UserReturn__, args...)                                              \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __UserReturn__                                                                                 \
  }

#define _ERRSTR(Obj, ExpR, __UserSS__)                                                             \
  if ((ExpR)) {                                                                                    \
    std::stringstream _CECS_SS_;                                                                   \
    __UserSS__;                                                                                    \
    (Obj).RecError_NoList(                                                                         \
        _CECS_DEFAULT_WARNID, _CECS_ERRTYPE_ERRSTR, __FNAME__, __LINE__, _CECS_SS_.str().c_str(),  \
        _CECS_SS_.str().size()                                                                     \
    );                                                                                             \
  }

#define _CHECKRT_ORG_(Obj)                                                                         \
  _ERRT(                                                                                           \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'throw' executed."                                       \
  )
#define _CHECKR_ORG_(Obj)                                                                          \
  CECS_ERR(                                                                                        \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return' executed."                                      \
  )
#define _CHECKRI_ORG_(Obj)                                                                         \
  CECS_ERRI(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return  _CECS_DEFAULT_ERRID(=%i)' executed.",           \
      _CECS_DEFAULT_ERRID                                                                          \
  )
#define _CHECKRN_ORG_(Obj)                                                                         \
  CECS_ERRN(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return NULL' executed."                                 \
  )
#define _CHECKRB_ORG_(Obj)                                                                         \
  CECS_ERRB(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return false' executed."                                \
  )
#define _CHECKRO_ORG_(Obj, __UserReturn__)                                                         \
  CECS_ERRO(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0, __UserReturn__,                                       \
      "CECS_CHECKERROR captured: __UserReturn__ code executed!."                                   \
  )


#define _CERRT_ORG(args...) {_ERRT(0 != _NERR_, args)}
#define _CERR_ORG(args...) {_ERR(0 != _NERR_, args)}
#define _CERRI_ORG(args...) {_ERRI(0 != _NERR_, args)}
#define _CERRN_ORG(args...) {_ERRN(0 != _NERR_, args)}
#define _CERRB_ORG(args...) {_ERRB(0 != _NERR_, args)}
#define _CERRO_ORG(__UserReturn__, args...) {_ERRO(0 != _NERR_, __UserReturn__, args)}
*/
#else
static_assert(1, "MSC compiler is not supported yet...");
#endif

// NOLINTEND
