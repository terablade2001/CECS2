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
  static CECSModule __ECSOBJ__("Module-A");

  DOCTEST_TEST_SUITE(
      "Test Modules"
  ) {
    TEST_CASE("Creating a CECSModule") {
      LOG_TEST_CASE("Test Modules", "Creating a CECSModule")
      auto &CECS = CECSSingleton::getInstance();
      if (CECS.state == CECSSingleton::State::NOT_INIT) { CECS.reconfigure(); }
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      try {
        // const int val = -938;
        // _ERRT(1, "This is an error throw message with id %i", val)
        _ERRT(1, "This is an error throw message")
        CHECK_EQ(0,1);
      } catch (const std::exception &e) {
        CHECK_EQ(1,1);
      }

      // _ERRT(1, "ERR01", "Error Throw!")
    }
  }
} // namespace docTests
