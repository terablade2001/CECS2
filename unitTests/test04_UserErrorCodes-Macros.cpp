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
      } catch (const std::exception &) { CHECK_EQ(1, CECS.getErrorIntegerAtExit()); }
      SUBCASE("Evaluate the AtExit ErrorCode Without _ECSCLS_ call") {
        try {
          _ERRTU(1, "UNDEFINED", "Testing an Undefined Error.")
          CHECK_EQ(2, 1);
        } catch (const std::exception &) {
          cout << "The AtExit errorCode should not change after this throw." << endl;
          CHECK_EQ(1, CECS.getErrorIntegerAtExit());
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
          CHECK_EQ(_CECS_MAGIC_THROW_ERRORCODE_, CECS.getErrorIntegerAtExit());
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
      } catch (const std::exception &) { CHECK_EQ(11, CECS.getErrorIntegerAtExit()); }

      try {
        _ERRTU(1, "TEST-ERROR-1011", "Testing with the custom OnIntReturn error 1011.")
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1011, CECS.getErrorIntegerOnIntReturn()); }
    }

    // TEST_CASE("_ERRIU with custon OnIntReturn codes") { // NOLINT
    //   LOG_TEST_CASE(
    //       "04 Test User Error Codes Macros",
    //       // NOLINTNEXTLINE
    //       "_ERRIU with custon OnIntReturn codes"
    //   )
    //   auto &CECS = CECSSingleton::getInstance();
    //   CHECK_NOTHROW(CECS.reconfigure());
    //   CHECK_EQ(0, CECS.getErrorIntegerAtExit());
    //   CHECK_NOTHROW(
    //       CECS.setNewErrorOnIntReturn("TEST-ERROR-1001", 1011, "Testing with a custom error
    //       1001")
    //   );
    //   cout << "--- Registered errors AtExit: " << endl;
    //   cout << CECS.getErrorsMapAtExit() << endl;
    //   cout << "--- Registered errors on integers returns: " << endl;
    //   cout << CECS.getErrorsMapOnIntReturn() << endl;
    //   try {
    //     _ERRTU(1, "TEST-ERROR-11", "Testing with the custom AtExit error 11.")
    //     CHECK_EQ(2, 1);
    //   } catch (const std::exception &) { CHECK_EQ(11, CECS.getErrorIntegerAtExit()); }
    //
    //   try {
    //     _ERRTU(1, "TEST-ERROR-1001", "Testing with the custom OnIntReturn error 11.")
    //     CHECK_EQ(2, 1);
    //   } catch (const std::exception &) { CHECK_EQ(1011, CECS.getErrorIntegerOnIntReturn()); }
    // }

    // --------------------------------------------------------------------------------------------
  }
} // namespace docTests
