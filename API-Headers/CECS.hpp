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

#define CECS_MODULE(moduleName) static CECSModule __ECSOBJ__(moduleName);
#define CECS_MAIN_MODULE(moduleName, projetName) static CECS __ECSOBJ__(moduleName, projectName);

#define _ECSCLS_ { CECSSingleton::resetNumberOfErrors(); }
#define _ECSCLS(numberOfLatestRecords) { CECSSingleton::resetNumberOfErrors(numberOfLatestRecords); }

#define _NERR_ (CECSSingleton::getNumberOfErrors())

#define CECS_MACRO_DISPATCHER(_1, _2, _3, _4, CECS_MACRO_DISPATCH_NAME, ...)                       \
  CECS_MACRO_DISPATCH_NAME

#ifndef _MSC_VER

#define _ERRT_ORG(ExpR, args...)                                                                   \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, args);                                                \
    __ECSOBJ__.throwErrors();                                                                      \
  }
#define _ERRT_EXT(ExpR, ErrId, args...)                                                            \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, ErrId, args);                                         \
    __ECSOBJ__.throwErrors();                                                                      \
  }
#define _ERRT(...) CECS_MACRO_DISPATCHER(__VA_ARGS__, _ERRT_ORG, _ERRT_EXT)(__VA_ARGS__)

/*
#define _ERR_ORG(Obj, ExpR, args...)                                                               \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN__                                                                                \
  }
#define _ERRI_ORG(Obj, ExpR, args...)                                                              \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_IRETURN__(_CECS_DEFAULT_ERRID)                                                          \
  }
#define _ERRN_ORG(Obj, ExpR, args...)                                                              \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN_NULL_                                                                            \
  }
#define _ERRB_ORG(Obj, ExpR, args...)                                                              \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN_BOOL_                                                                            \
  }
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
