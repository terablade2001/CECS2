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
      "MODULE:Test03"
  )

  DOCTEST_TEST_SUITE(
      "03 Test Message logging Macros"
  ) {
    TEST_CASE("Testing the basic HLog_() macro with formatted messages") {
      LOG_TEST_CASE(
          "03 Test Message logging Macros",
          "Testing the basic HLog_() macro with formatted messages"
      )
      auto &CECS                 = CECSSingleton::getInstance();
      auto  configuration        = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      configuration.logFileName  = "CECSLog.log";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      try {
        HLog_(
            L::TRC, "This is a trace message with fileLogLevel = %i",
            static_cast<int>(configuration.fileLogLevel)
        );
        ostringstream oss;
        oss << "This is a debug message with logCustomFormatForFile = "
            << configuration.logCustomFormatForFile;
        HLog_(L::DBG, oss.str());
        HLog_(
            L::WARN, "This is a warning message with logFileName = %s",
            configuration.logFileName.c_str()
        );
        HLog_(
            L::ERR, "This is an error message with logCustomFormatForScreen = %s",
            configuration.logCustomFormatForScreen.c_str()
        );
        CHECK_EQ(1, 1);
      } catch (const std::exception &) { CHECK_EQ(2, 1); }
      try {
        HLog_(L::CRIT, "This is a critical message");
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
    }

    TEST_CASE("Testing the basic ILogs_() macro (deprecated: compatibility only)") {
      LOG_TEST_CASE(
          "03 Test Message logging Macros",
          "Testing the basic ILogs_() macro (deprecated: compatibility only)"
      )
      auto &CECS          = CECSSingleton::getInstance();
      auto  configuration = CECS.getConfiguration();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      ostringstream oss;
      oss << "ILogs_() Test: This is a message with logCustomFormatForFile = "
          << configuration.logCustomFormatForFile;
      try {
        ILogs_(L::DBG, oss.str());
        CHECK_EQ(1, 1);
      } catch (const std::exception &) { CHECK_EQ(2, 1); }
      try {
        ILogs_(L::CRIT, oss.str());
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
    }

    TEST_CASE("Testing the basic ILog_() macro, which adds module and line number.") {
      LOG_TEST_CASE(
          "03 Test Message logging Macros",
          "Testing the basic ILogs_() macro, which adds module and line number."
      )
      auto &CECS                 = CECSSingleton::getInstance();
      auto  configuration        = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      configuration.logFileName  = "CECSLog.log";
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      try {
        ILog_(
            L::TRC, "This is a trace message with fileLogLevel = %i",
            static_cast<int>(configuration.fileLogLevel)
        );
        ostringstream oss;
        oss << "This is a debug message with logCustomFormatForFile = "
            << configuration.logCustomFormatForFile;
        ILog_(L::DBG, oss.str());
        ILog_(
            L::WARN, "This is a warning message with logFileName = %s",
            configuration.logFileName.c_str()
        );
        ILog_(
            L::ERR, "This is an error message with logCustomFormatForScreen = %s",
            configuration.logCustomFormatForScreen.c_str()
        );
        CHECK_EQ(1, 1);
      } catch (const std::exception &) { CHECK_EQ(2, 1); }
      try {
        ILog_(L::CRIT, "This is a critical message");
        CHECK_EQ(2, 1);
      } catch (const std::exception &) { CHECK_EQ(1, 1); }
    }
  }
} // namespace docTests
