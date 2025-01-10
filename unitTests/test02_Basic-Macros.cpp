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
  static CECSModule __ECSOBJ__("MODULE:Test02");

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
    _ERRSTR(min < max, {
      ss << "This is a note that min is less than max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    _ERRO(min < max, { return string{"Min<Max"}; }, "Min[=%i] < Max[=%i]", min, max)
    _ERRSTR(min > max, {
      ss << "This is a note that min is greater than max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    _ERRO(min > max, { return string{"Min>Max"}; }, "Min[=%i] > Max[=%i]", min, max)
    _ERRSTR(min == max, {
      ss << "This is a note that min is equal to max!";
      ss << "\n - min: " << min;
      ss << "\n - max: " << max;
    })
    return str;
  }

  bool test02ErrFunc03(
      const int x
  ) {
    _ERRB(x < 0, "test02ErrFunc03(): x[=%i] < 0", x)
    return true;
  }

  // NOLINTNEXTLINE
  void *test02ErrFunc02(
      const int x
  ) {
    _ERRN(!test02ErrFunc03(x), "test02ErrFunc03() Failed")
    return nullptr;
  }

  int test02ErrFunc01(
      const int x
  ) {
    test02ErrFunc02(x);
    _CERRI_
    return 0;
  }

  DOCTEST_TEST_SUITE(
      "02 Test Basic Macros"
  ) {
    TEST_CASE("Checking the _ERRT Macro that properly records and throw") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRT Macro that properly records and throw")
      auto &CECS                             = CECSSingleton::getInstance();
      auto  configuration                    = CECS.getConfiguration();
      configuration.logCustomFormatForScreen = "[%^-%L-%$] %v";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      try {
        constexpr int val = -938;
        _ERRT(1, "This is an error throw message with id %i", val)
        CHECK_EQ(0, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
      CHECK_EQ(1, CECSSingleton::getNumberOfErrors());
    }


    TEST_CASE("Checking the _ERRI Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRT Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      int err;
      err = retFunction(1, 2);
      CHECK_EQ(0, err);
      err = retFunction(2, 2);
      CHECK_EQ(0, err);
      err = retFunction(3, 2);
      CHECK_NE(0, err);
      CHECK_EQ(1, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking the _ERR Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERR Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      int res = 0;
      retFunctionR(1, 2, res);
      CHECK_EQ(1, res);
      retFunctionR(3, 2, res);
      CHECK_EQ(2, res);
      retFunctionR(2, 2, res);
      CHECK_EQ(3, res);
      CHECK_EQ(2, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking the _ERRN Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRN Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      int res = 0;
      retFunctionR(1, 2, res);
      CHECK_EQ(1, res);
      retFunctionR(3, 2, res);
      CHECK_EQ(2, res);
      retFunctionR(2, 2, res);
      CHECK_EQ(3, res);
      CHECK_EQ(2, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking the _ERRB Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRB Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      bool res{false};
      res = retFunctionB(1, 2);
      CHECK_EQ(true, res);
      res = retFunctionB(2, 2);
      CHECK_EQ(true, res);
      res = retFunctionB(3, 2);
      CHECK_EQ(false, res);
      CHECK_EQ(1, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking the _ERRO Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRO Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      std::string s;
      s = retFunctionS(1, 1);
      CHECK_EQ("Hello World", s);
      s = retFunctionS(1, 2);
      CHECK_EQ("Min<Max", s);
      s = retFunctionS(2, 1);
      CHECK_EQ("Min>Max", s);
      CHECK_EQ(2, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking the _ERRSTR Macro that properly records") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking the _ERRSTR Macro that properly records")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      std::string s;
      s = retFunctionS2(1, 1);
      CHECK_EQ("Hello World", s);
      s = retFunctionS2(1, 2);
      CHECK_EQ("Min<Max", s);
      s = retFunctionS2(2, 1);
      CHECK_EQ("Min>Max", s);
      CHECK_EQ(2, CECSSingleton::getNumberOfErrors());
    }

    TEST_CASE("Checking nested functions errors in CRITICAL mode") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking nested functions errors in CRITICAL mode")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      int err;
      err = test02ErrFunc01(1);
      CHECK_EQ(0, err);
      err = test02ErrFunc01(-1);
      CHECK_NE(0, err);
      cout << "CECSSingleton::getNumberOfErrors() = " << CECSSingleton::getNumberOfErrors() << endl;
      SUBCASE("Try to clean the errors while in CRITICAL mode ... It should throw.") {
        try {
          _ECSCLS_
          CHECK_EQ(0, 1);
        } catch (const std::exception &e) {
          cout << e.what() << endl;
          CHECK_EQ(1, 1);
        }
      }
    }

    /**
     * @test ErrorMode ERROR which is set using _CECS_MODE_ERR_ can be used to test if some code
     * passes and in the case it does not passes, to be able to clear the errors and commit other
     * actions, like for instance initiate a debug code (exporting data etc.).
     * In this case all captured errors are logged as ERROR, instead of CRITICAL.
     * While in this mode the `_ECSCLS_` and `_ECSCLS(n)` macros are working.
     * In CRITICAL mode (default mode) the use of these functions will throw.
     */
    TEST_CASE("Checking nested functions errors in ERROR Mode") {
      LOG_TEST_CASE("02 Test Basic Macros", "Checking nested functions errors in ERROR Mode")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CECSSingleton::resetNumberOfErrors();
      _CECS_MODE_ERR_
      int err;
      err = test02ErrFunc01(1);
      CHECK_EQ(0, err);
      err = test02ErrFunc01(-1);
      CHECK_NE(0, err);
      cout << "CECSSingleton::getNumberOfErrors() = " << CECSSingleton::getNumberOfErrors() << endl;
      SUBCASE("Try to clean the errors while in ERROR mode ... It should not throw.") {
        try {
          _ECSCLS_
          CHECK_EQ(1, 1);
          cout << "CECSSingleton::getNumberOfErrors() after _ECSCLS_ = "
               << CECSSingleton::getNumberOfErrors() << endl;
        } catch (const std::exception &e) {
          cout << e.what() << endl;
          CHECK_EQ(0, 1);
        }
      }
      _CECS_MODE_CRIT_
    }
  }
} // namespace docTests
