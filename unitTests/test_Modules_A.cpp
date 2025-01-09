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
    TEST_CASE("Testing _ERRT macro") {
      LOG_TEST_CASE("Test Modules", "Testing that _ERRT throws and records properly")
      auto &CECS                         = CECSSingleton::getInstance();
      CECS.configuration.logCustomFormat = "[%^%L%$] %v";
      CECS.reconfigure();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);

      {
        std::ostringstream captured_stderr;
        std::streambuf    *original_cerr_buffer = std::cerr.rdbuf();
        std::cerr.rdbuf(captured_stderr.rdbuf());
        int line = 0;
        try {
          constexpr int val = -938;
          line              = __LINE__;
          _ERRT(1, "This is an error throw message with id %i", val)
          CHECK_EQ(0, 1);
        } catch (const std::exception &) { CHECK_EQ(1, 1); }
        std::cerr.rdbuf(original_cerr_buffer);
        ostringstream oss;
        oss << "\n[C] (" << __FNAME__ << ", L-" << line
            << "): This is an error throw message with id -938";
        string s(captured_stderr.str());
        cout << "[" << s << "]" << endl;
        CHECK_EQ(0,captured_stderr.str().compare(oss.str()));
      }
    }
  }
} // namespace docTests
