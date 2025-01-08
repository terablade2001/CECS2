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
    TEST_CASE("Test CECS Singleton") {
      LOG_TEST_CASE("Basic Operations", "CECS has default argument")
      std::lock_guard<std::mutex> lock(testMutex);
      auto                       &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.getECSName(), "CECS-Default");
      SUBCASE("Changing the CECSSingleton name") {
        CECS.setECSName("Test");
        CHECK_EQ(CECS.getECSName(), "Test");
      }
      std::remove("CECSLog.log");
    }

    TEST_CASE("Configure CECS Singleton") {
      LOG_TEST_CASE("Basic Operations", "Trying to configure CECS singleton")
      std::lock_guard<std::mutex> lock(testMutex);
      SUBCASE("Throw error if logger name provided is empty") {
        auto &CECS = CECSSingleton::getInstance();

        CECSConfiguration invalidConfig;
        invalidConfig.loggerName = "";
        CHECK_THROWS_AS(CECS.setECSConfiguration(invalidConfig), std::invalid_argument);
        CECS.Shutdown();
      }
      SUBCASE("Work and enforce generation of logger file") {
        remove("TestConfigCECSSingleton.log");
        auto             &CECS = CECSSingleton::getInstance();
        CECSConfiguration invalidConfig;
        invalidConfig.loggerName   = "SomeName";
        invalidConfig.logFileName  = "TestConfigCECSSingleton.log";
        invalidConfig.fileLogLevel = 3;
        CHECK_NOTHROW(CECS.setECSConfiguration(invalidConfig));
        CHECK_EQ(true, isFileExist(invalidConfig.logFileName));
        CECS.Shutdown();
        int err = remove("TestConfigCECSSingleton.log");
        CHECK_EQ(err, 0);
        std::remove("CECSLog.log");
      }
      SUBCASE(
          "Check if CECS automatically creates the default file after accessing it's instance"
      ) {
        // This ensures that at this moment we don't have any file generated.
        CHECK_EQ(false, isFileExist("CECSLog.log"));
        {
          auto &CECS = CECSSingleton::getInstance();
          // Modify the default config to test different cases.
          auto &defaultConfig = CECS.defaultConfiguration;
          if (defaultConfig.fileLogLevel == 0) {
            CHECK_EQ(false, isFileExist(defaultConfig.logFileName));
          } else if (defaultConfig.fileLogLevel > 0) {
            CHECK_EQ(true, isFileExist(defaultConfig.logFileName));
          }
        }
        std::remove("CECSLog.log");
      }
    }
  }

} // namespace docTests
