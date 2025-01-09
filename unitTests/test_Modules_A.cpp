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

  void retFunctionR(
      const int min, const int max, int &res
  ) {
    res = 1;
    _ERR(min < max, "Min[=%i] < Max[=%i]", min, max)
    res = 2;
    _ERR(min > max, "Min[=%i] > Max[=%i]", min, max)
    res = 3;
  }

  // NOLINTNEXTLINE
  void *retFunctionN(
      const int min, const int max, int &res
  ) {
    res = 1;
    _ERRN(min < max, "_ERRN: Min[=%i] < Max[=%i]", min, max)
    res = 2;
    _ERRN(min > max, "_ERRN: Min[=%i] > Max[=%i]", min, max)
    res = 3;
    return nullptr;
  }

  bool retFunctionB(
      const int min, const int max
  ) {
    _ERRB(min > max, "Min[=%i] > Max[=%i]", min, max)
    return true;
  }

  std::string retFunctionS(
      const int min, const int max
  ) {
    std::string str{"Hello World"};
    _ERRO(min < max, { return string{"Min<Max"}; }, "Min[=%i] < Max[=%i]", min, max)
    _ERRO(min > max, { return string{"Min>Max"}; }, "Min[=%i] > Max[=%i]", min, max)
    return str;
  }

  std::string retFunctionS2(
    const int min, const int max
) {
    std::string str{"Hello World"};
    _ERRSTR(min < max,{
      ss << "This is a note that min is less than max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    _ERRO(min < max, { return string{"Min<Max"}; }, "Min[=%i] < Max[=%i]", min, max)
    _ERRSTR(min > max,{
      ss << "This is a note that min is greater than max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    _ERRO(min > max, { return string{"Min>Max"}; }, "Min[=%i] > Max[=%i]", min, max)
    _ERRSTR(min == max,{
      ss << "This is a note that min is equal to max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    return str;
  }
  DOCTEST_TEST_SUITE(
      "Test Modules"
  ) {
    TEST_CASE("Checking the _ERRT Macro that properly records and throw") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRT Macro that properly records and throw")
      auto &CECS                            = CECSSingleton::getInstance();
      CECS.configuration.useLogCustomFormat = true;
      CECS.configuration.logCustomFormat    = "[%^%L%$] %v";
      CECS.reconfigure();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      try {
        constexpr int val = -938;
        _ERRT(1, "This is an error throw message with id %i", val)
        CHECK_EQ(0, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
    }


    TEST_CASE("Checking the _ERRI Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRT Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      int err = 0;
      err     = retFunction(1, 2);
      CHECK_EQ(0, err);
      err = retFunction(2, 2);
      CHECK_EQ(0, err);
      err = retFunction(3, 2);
      CHECK_NE(0, err);
    }

    TEST_CASE("Checking the _ERR Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERR Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      int res = 0;
      retFunctionR(1, 2, res);
      CHECK_EQ(1, res);
      retFunctionR(3, 2, res);
      CHECK_EQ(2, res);
      retFunctionR(2, 2, res);
      CHECK_EQ(3, res);
    }

    TEST_CASE("Checking the _ERRN Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRN Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      int res = 0;
      retFunctionR(1, 2, res);
      CHECK_EQ(1, res);
      retFunctionR(3, 2, res);
      CHECK_EQ(2, res);
      retFunctionR(2, 2, res);
      CHECK_EQ(3, res);
    }

    TEST_CASE("Checking the _ERRB Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRB Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      bool res{false};
      res = retFunctionB(1, 2);
      CHECK_EQ(true, res);
      res = retFunctionB(2, 2);
      CHECK_EQ(true, res);
      res = retFunctionB(3, 2);
      CHECK_EQ(false, res);
    }

    TEST_CASE("Checking the _ERRO Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRO Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      std::string s;
      s = retFunctionS(1, 1);
      CHECK_EQ("Hello World", s);
      s = retFunctionS(1, 2);
      CHECK_EQ("Min<Max", s);
      s = retFunctionS(2, 1);
      CHECK_EQ("Min>Max", s);
    }

    TEST_CASE("Checking the _ERRSTR Macro that properly records") {
      LOG_TEST_CASE("Test Modules", "Checking the _ERRSTR Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      std::string s;
      s = retFunctionS2(1, 1);
      CHECK_EQ("Hello World", s);
      s = retFunctionS2(1, 2);
      CHECK_EQ("Min<Max", s);
      s = retFunctionS2(2, 1);
      CHECK_EQ("Min>Max", s);
    }
  }
} // namespace docTests
