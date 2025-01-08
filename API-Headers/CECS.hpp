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


#include <CECSSingleton.hpp>


#ifndef __FNAME__
#define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__)
#endif

#define CECS_MODULE(moduleName) static CECSModule __ECSOBJ__(moduleName);
#define CECS_MAIN_MODULE(moduleName, projetName) static CECS __ECSOBJ__(moduleName, projectName);

#define _ECSCLS_ { __ECSOBJ__.clear(); }
#define _ECSCLS(numberOfLatestRecords) { __ECSOBJ__.clear((numberOfLatestRecords)); }

#define _NERR_ (__ECSOBJ__.GetNumberOfErrors())
#define _NERR(x) (__ECSOBJ__.GetNumberOfErrors(x))

#ifndef _MSC_VER

#define _ERRT(ExpR, args...)                                                                       \
  if ((ExpR)) {                                                                                    \
    __ECSOBJ__.RecError(__FNAME__, __LINE__, args);                                                \
    __CECS_THROW__(Obj)                                                                            \
  }
#define _ERR(Obj, ExpR, args...)                                                                   \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN__                                                                                \
  }
#define _ERRI(Obj, ExpR, args...)                                                                  \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_IRETURN__(_CECS_DEFAULT_ERRID)                                                          \
  }
#define _ERRN(Obj, ExpR, args...)                                                                  \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN_NULL_                                                                            \
  }
#define _ERRB(Obj, ExpR, args...)                                                                  \
  if ((ExpR)) {                                                                                    \
    (Obj).RecError(_CECS_DEFAULT_ERRID, _CECS_ERRTYPE_ERROR, __FNAME__, __LINE__, args);           \
    __CECS_RETURN_BOOL_                                                                            \
  }
#define _ERRO(Obj, ExpR, __UserReturn__, args...)                                                  \
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

#define _CHECKRT_(Obj)                                                                             \
  _ERRT(                                                                                           \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'throw' executed."                                       \
  )
#define _CHECKR_(Obj)                                                                              \
  CECS_ERR(                                                                                        \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return' executed."                                      \
  )
#define _CHECKRI_(Obj)                                                                             \
  CECS_ERRI(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return  _CECS_DEFAULT_ERRID(=%i)' executed.",           \
      _CECS_DEFAULT_ERRID                                                                          \
  )
#define _CHECKRN_(Obj)                                                                             \
  CECS_ERRN(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return NULL' executed."                                 \
  )
#define _CHECKRB_(Obj)                                                                             \
  CECS_ERRB(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0,                                                       \
      "CECS_CHECKERROR captured: Function 'return false' executed."                                \
  )
#define _CHECKRO_(Obj, __UserReturn__)                                                             \
  CECS_ERRO(                                                                                       \
      (Obj), (Obj).GetNumberOfErrors() != 0, __UserReturn__,                                       \
      "CECS_CHECKERROR captured: __UserReturn__ code executed!."                                   \
  )


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
