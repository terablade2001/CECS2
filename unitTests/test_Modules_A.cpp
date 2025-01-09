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

  int retFunction(
      const int min, const int max
  ) {
    _ERRI(min > max, "Min[=%i] > Max[=%i]", min, max)
    return 0;
  }

  DOCTEST_TEST_SUITE(
      "Test Modules"
  ) {
    TEST_CASE("Checking the _ERRT Macro that properly records and throw") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRT Macro that properly records and throw")
      auto &CECS = CECSSingleton::getInstance();
      if (CECS.state == CECSSingleton::State::NOT_INIT) { CECS.reconfigure(); }
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      try {
        constexpr int val = -938;
        _ERRT(1, "This is an error throw message with id %i", val)
        CHECK_EQ(0, 1);
      } catch (const std::exception &e) { CHECK_EQ(1, 1); }
    }
  }

  TEST_CASE("Checking the _ERRI Macro that properly records") {
    LOG_TEST_CASE("Test Modules", "Checking the _ERRT Macro that properly records")
    auto &CECS = CECSSingleton::getInstance();
    if (CECS.state == CECSSingleton::State::NOT_INIT) { CECS.reconfigure(); }
    CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

    int err = 0;
    err = retFunction(1, 2);
    CHECK_EQ(0, err);
    err = retFunction(2, 2);
    CHECK_EQ(0, err);
    err = retFunction(3, 2);
    CHECK_NE(0, err);
  }
} // namespace docTests
