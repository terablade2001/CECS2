#include <CECS.hpp>
#include <TestsDescriptionLogger.hpp>

// NOLINTBEGIN
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <ftw.h>
#ifdef __unix__
#include <sys/stat.h>
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif
// NOLINTEND



namespace docTests {
  using namespace std;
  using namespace Logger;
  CECS_MODULE(
      "MODULE:Test04"
  )

  int functionPositiveMinMax(
      const int min, const int max
  ) {
    _ERRIU((min == -1)&&(max==-1), "UNDEFINED-TEST-CASE", "Min == Max == -1")
    _ERRIU(min < 0, "NEGATIVE NUMBER DETECTED", "Min [=%i] < 0", min)
    _ERRIU(max < 0, "NEGATIVE NUMBER DETECTED", "Max [=%i] < 0", max)
    _ERRIU(min > max, "MIN > MAX", "Min [=%i] > Max [=%i]", min, max)
    _ERRIU(min == max, "MIN == MAX", "Min [=%i] == Max [=%i]", min, max)
    // Do something ...
    return 0;
  }

  DOCTEST_TEST_SUITE(
      "04 Test User Error Codes Macros"
  ) {
    TEST_CASE(
        // NOLINTNEXTLINE
        "Testing _ERRTU GENERIC and UNDEFINED codes. Evaluation when also clearing them or not."
    ) {
      LOG_TEST_CASE(
          "04 Test User Error Codes Macros",
          // NOLINTNEXTLINE
          "Testing _ERRTU GENERIC and UNDEFINED codes. Evaluation when also clearing them or not."
      )
      auto &CECS                 = CECSSingleton::getInstance();
      auto  configuration        = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      configuration.logFileName  = "CECSLog.log";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      try {
        _ERRTU(1, "GENERIC", "Testing Generic Error.")
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1, _CECS_CODE_ATEXIT_); }
      SUBCASE("Evaluate the AtExit ErrorCode Without _ECSCLS_ call") {
        try {
          _ERRTU(1, "UNDEFINED", "Testing an Undefined Error.")
          CHECK_EQ(2, 1);
        } catch (const std::exception &) {
          cout << "The AtExit errorCode should not change after this throw." << endl;
          CHECK_EQ(1, _CECS_CODE_ATEXIT_);
        }
      }
      SUBCASE("Evaluate the AtExit ErrorCode With _ECSCLS_ call") {
        _CECS_MODE_ERR_
        _ECSCLS_
        _CECS_MODE_CRIT_
        try {
          _ERRTU(1, "UNDEFINED", "Testing an Undefined Error.")
          CHECK_EQ(2, 1);
        } catch (const std::exception &) {
          cout << "The AtExit errorCode should provide the MAGIC THROW ERROR CODE because "
                  "UNDEFINED error has not been defined in the error map at this point."
               << endl;
          CHECK_EQ(_CECS_MAGIC_THROW_ERRORCODE_, _CECS_CODE_ATEXIT_);
        }
      }
    }

    TEST_CASE("_ERRTU with adding custom AtExit and OnIntReturn error codes") { // NOLINT
      LOG_TEST_CASE(
          "04 Test User Error Codes Macros",
          // NOLINTNEXTLINE
          "_ERRTU with adding custom AtExit and OnIntReturn error codes"
      )
      auto &CECS = CECSSingleton::getInstance();
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(0, CECS.getErrorIntegerAtExit());
      CHECK_NOTHROW(CECS.setNewErrorAtExit("TEST-ERROR-11", 11, "Testing with a custom error 11"));
      CHECK_NOTHROW(
          CECS.setNewErrorOnIntReturn("TEST-ERROR-1011", 1011, "Testing with a custom error 1011")
      );
      cout << "--- Registered errors AtExit: " << endl;
      cout << CECS.getErrorsMapAtExit() << endl;
      cout << "--- Registered errors on integers returns: " << endl;
      cout << CECS.getErrorsMapOnIntReturn() << endl;
      try {
        _ERRTU(1, "TEST-ERROR-11", "Testing with the custom AtExit error 11.")
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(11, _CECS_CODE_ATEXIT_); }

      try {
        _ERRTU(1, "TEST-ERROR-1011", "Testing with the custom OnIntReturn error 1011.")
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1011, _CECS_CODE_ONINTRETURN_); }
    }

    TEST_CASE("_ERRIU with custon OnIntReturn codes") { // NOLINT
      LOG_TEST_CASE(
          "04 Test User Error Codes Macros",
          // NOLINTNEXTLINE
          "_ERRIU with custon OnIntReturn codes"
      )
      auto &CECS = CECSSingleton::getInstance();
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(0, CECS.getErrorIntegerAtExit());
      CHECK_NOTHROW(CECS.setNewErrorOnIntReturn(
          "MIN > MAX", 1011, "Minimum value provides is larger than Maximum value provided!"
      ));
      CHECK_NOTHROW(CECS.setNewErrorOnIntReturn(
          "MIN == MAX", 1012, "Minimum value provides is equal to Maximum value provided!"
      ));
      CHECK_NOTHROW(CECS.setNewErrorOnIntReturn(
          "NEGATIVE NUMBER DETECTED", 1013, "A Negative number was detected."
      ));
      cout << "--- Registered errors on integers returns: " << endl;
      cout << CECS.getErrorsMapOnIntReturn() << endl;
      int retCode;

      // Using ERR mode to clear the errors on demand. If not clear the first error occurred is
      // kept.
      _CECS_MODE_ERR_

      retCode = functionPositiveMinMax(10, 5);
      CHECK_EQ(retCode, 1011);
      cout << "Expected 1011 error code if min > max. Result: " << retCode << endl;

      retCode = functionPositiveMinMax(10, 10);
      CHECK_EQ(retCode, 1011);
      cout << "Expected 1011 error code if min == max, because we have not cleared the errors. "
              "Result: "
           << retCode << endl;
      cout << "Cleaning recorded errors... " << endl;
      _ECSCLS_

      retCode = functionPositiveMinMax(10, 10);
      CHECK_EQ(retCode, 1012);
      cout << "Expected 1012 error code if min == max. Result: " << retCode << endl;
      cout << "Cleaning recorded errors... " << endl;
      _ECSCLS_

      retCode = functionPositiveMinMax(-1, 10);
      CHECK_EQ(retCode, 1013);
      cout << "Cleaning recorded errors... " << endl;
      _ECSCLS_

      retCode = functionPositiveMinMax(10, -10);
      CHECK_EQ(retCode, 1013);
      cout << "Cleaning recorded errors... " << endl;
      _ECSCLS_

      retCode = functionPositiveMinMax(-99, -110);
      CHECK_EQ(retCode, 1013);
      cout << "Expected 1013 error code if min or max are less than 0. Result: " << retCode << endl;
      cout << "Cleaning recorded errors... " << endl;
      _ECSCLS_

      retCode = functionPositiveMinMax(5, 10);
      CHECK_EQ(retCode, 0);
      cout << "Expected 0 error code if min < max. Result: " << retCode << endl;
      _ECSCLS_

      // Try with a code tag which is not defined!
      try {
        retCode = functionPositiveMinMax(-1, -1);
        CHECK_EQ(1, 0); // The above should throw!
      } catch (std::exception &) {
        CHECK_EQ(retCode, 0);
        cout << "Expected 0 error code for 'UNDEFINED-TEST_CASE'. Result: " << retCode << endl;
        CHECK_EQ(_CECS_MAGIC_THROW_ERRORCODE_, _CECS_CODE_ATEXIT_);
        cout << "Unlisted error code is correctly detected!" << endl;
        cout << "Due to 'UNDEFINED-TEST_CASE' we expect AtExit error code to "<<_CECS_MAGIC_THROW_ERRORCODE_<<". Result: " << _CECS_CODE_ATEXIT_ << endl;
      }
      _ECSCLS_

      _CECS_MODE_CRIT_
    }
  }

  // --------------------------------------------------------------------------------------------
} // namespace docTests
