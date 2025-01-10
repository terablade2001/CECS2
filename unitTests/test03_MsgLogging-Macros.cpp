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
  static CECSModule __ECSOBJ__("MODULE:Test03");

  DOCTEST_TEST_SUITE(
      "03 Test Message logging Macros"
  ) {
    TEST_CASE("Testing the basic HLog_() macro with C-formatted message") {
      LOG_TEST_CASE(
          "03 Test Message logging Macros",
          "Testing the basic HLog_() macro with C-formatted message"
      )
      auto &CECS                 = CECSSingleton::getInstance();
      auto  configuration        = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      configuration.logFileName  = "CECSLog.log";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      try {
        HLog_(L::TRC, "This is a trace message with fileLogLevel = %i", static_cast<int>(configuration.fileLogLevel));
        ostringstream oss; oss << "This is a debug message with logCustomFormatForFile = " <<configuration.logCustomFormatForFile;
        HLog_(L::DBG, oss.str());
        HLog_(L::WARN, "This is a warning message with logFileName = %s", configuration.logFileName.c_str());
        HLog_(L::ERR, "This is an error message with logCustomFormatForScreen = %s", configuration.logCustomFormatForScreen.c_str());
        CHECK_EQ(1, 1);
      } catch (const std::exception &) { CHECK_EQ(2, 1); }
      try {
        HLog_(L::CRIT, "This is a critical message");
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
    }
  }
} // namespace docTests
