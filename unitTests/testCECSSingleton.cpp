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
  static std::mutex testMutex;
  using namespace std;

  bool isFileExist(
      const std::string &filepath_
  ) {
    FILE *fp = fopen(filepath_.c_str(), "r");
    if (fp == nullptr) return false;
    fclose(fp);
    return true;
  }

  DOCTEST_TEST_SUITE(
      "Basic Operations"
  ) {
    TEST_CASE("Renaming the CECS Singleton") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("Basic Operations", "Testing that the CECS singleton can be renamed")
      //.
      {
        auto &CECS = CECSSingleton::getInstance();
        CHECK_EQ(CECS.getECSName(), "CECS-Default");
        SUBCASE("Changing the CECSSingleton name") {
          CECS.setECSName("Test");
          CHECK_EQ(CECS.getECSName(), "Test");
        }
        CECS.Shutdown();
      }
      std::remove("CECSLog.log");
    }

    TEST_CASE("Configure CECS Singleton with no name should throw") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("Basic Operations", "Configure CECS Singleton with no name should throw") {
        auto &CECS = CECSSingleton::getInstance();

        CECSConfiguration invalidConfig;
        invalidConfig.loggerName = "";
        CHECK_THROWS_AS(CECS.setECSConfiguration(invalidConfig), std::invalid_argument);
        CECS.Shutdown();
      }
      std::remove("CECSLog.log");
    }

    TEST_CASE("Confirm CECS Singleton generates custom log file after reConfigured") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE(
          "Basic Operations", "Confirm CECS Singleton generates custom log file after reConfigured"
      )
      //.
      remove("TestConfigCECSSingleton.log");
      {
        auto             &CECS = CECSSingleton::getInstance();
        CECSConfiguration invalidConfig;
        invalidConfig.loggerName   = "SomeName";
        invalidConfig.logFileName  = "TestConfigCECSSingleton.log";
        invalidConfig.fileLogLevel = 3;
        CHECK_NOTHROW(CECS.setECSConfiguration(invalidConfig));
        CHECK_EQ(true, isFileExist(invalidConfig.logFileName));
        CECS.Shutdown();
      }
      int err = remove("TestConfigCECSSingleton.log");
      CHECK_EQ(err, 0);
      std::remove("CECSLog.log");
    }

    TEST_CASE("Test if CECS Singleton Generates or Not the default file based on default config") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE(
          "Basic Operations",
          "Test if CECS Singleton Generates or Not the default file based on default config"
      )
      {
        auto &CECS = CECSSingleton::getInstance();
        // Modify the default config to test different cases.
        auto &defaultConfig = CECS.defaultConfiguration;
        if (defaultConfig.fileLogLevel == 0) {
          CHECK_EQ(false, isFileExist(defaultConfig.logFileName));
        } else if (defaultConfig.fileLogLevel > 0) {
          CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
          CHECK_EQ(true, isFileExist(defaultConfig.logFileName));
        }
        CECS.Shutdown();
      }
      std::remove("CECSLog.log");
    }
  }

} // namespace docTests
