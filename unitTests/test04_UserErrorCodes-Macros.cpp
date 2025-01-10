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
    TEST_CASE("Testing _ERRTU with custom error code") { // NOLINT
      // NOLINTNEXTLINE
      LOG_TEST_CASE("04 Test User Error Codes Macros", "Testing _ERRTU with custom error code")
      auto &CECS                 = CECSSingleton::getInstance();
      auto  configuration        = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      configuration.logFileName  = "CECSLog.log";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      try {
        _ERRTU(1,"GENERIC","Testing Generic Error.")
        CHECK_EQ(2, 1);
      } catch (const std::exception &) {
        CHECK_EQ(1, CECS.getErrorIntegerAtExit());
      }
    }
  }
} // namespace docTests
