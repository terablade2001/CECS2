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
  static CECSModule __ECSOBJ__("Module-Test03");

  DOCTEST_TEST_SUITE(
      "03 Different Sink Patterns"
  ) {
//    TEST_CASE("Log with different pattern in screen and file") {
//      LOG_TEST_CASE("03 Different Sink Patterns", "Log with different pattern in screen and file")
//      auto &CECS                            = CECSSingleton::getInstance();
//      CECS.configuration.useLogCustomFormat = true;
//      CECS.configuration.logCustomFormat    = "[%^%L%$] %v";
//      CECS.reconfigure();
//      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
//      CECSSingleton::resetNumberOfErrors();
//      try {
//        constexpr int val = -938;
//        _ERRT(1, "This is an error throw message with id %i", val)
//        CHECK_EQ(0, 1);
//      } catch (const std::exception &) { CHECK_EQ(1, 1); }
//      CHECK_EQ(1, CECSSingleton::getNumberOfErrors());
//    }

  }
} // namespace docTests
