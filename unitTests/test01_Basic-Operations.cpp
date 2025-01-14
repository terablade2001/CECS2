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
  static std::mutex testMutex;
  CECS_MAIN_MODULE(
      "MODULE:Test01", "UnitTesting"
  )

  bool isFileExist(
      const std::string &filepath_
  ) {
    FILE *fp = fopen(filepath_.c_str(), "r");
    if (fp == nullptr) return false;
    fclose(fp);
    return true;
  }

  DOCTEST_TEST_SUITE(
      "01 Basic Operations"
  ) {
    TEST_CASE("Check Version") {
      LOG_TEST_CASE("01 Basic Operations", "Check Version")
      cout << "CECS Version: " << CECSSingleton::getCECSVersion() << endl;
#include "../version.h"



      CHECK_EQ(PROJECT_VERSION, CECSSingleton::getCECSVersion());
      CHECK_EQ(PROJECT_VERSION_MAJOR, CECSSingleton::getCECSVersionMajor());
      CHECK_EQ(PROJECT_VERSION_MINOR, CECSSingleton::getCECSVersionMinor());
      CHECK_EQ(PROJECT_VERSION_PATCH, CECSSingleton::getCECSVersionPatch());
    }

    TEST_CASE("Renaming the CECS Singleton") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("01 Basic Operations", "Renaming the CECS Singleton")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CHECK_NE(CECS.getProjectName(), "CECS-Default"); // We have used (CECS_MAIN_MODULE)
      cout << "--- PROJECT :: " << CECS.getProjectName() << " ---" << endl;
      SUBCASE("Changing the CECSSingleton name") {
        CECS.setProjectName("Test");
        CHECK_EQ(CECS.getProjectName(), "Test");
      }
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }

    TEST_CASE("Configure CECS Singleton with no name should throw") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("01 Basic Operations", "Configure CECS Singleton with no name should throw")
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually reconfigure it.
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECS.reconfigure();
        CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      }


      CECSSingleton::Configuration invalidConfig;
      // User must not use initializeLogger()
      CHECK_THROWS_AS(CECS.initializeLogger(invalidConfig), std::runtime_error);
      invalidConfig.loggerName = "";
      CECS.setConfiguration(invalidConfig);
      CHECK_THROWS_AS(CECS.reconfigure(), std::invalid_argument);
      invalidConfig.loggerName = "CECS";
      CECS.setConfiguration(invalidConfig);
      CHECK_NOTHROW(CECS.reconfigure());
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }

    TEST_CASE("Confirm CECS Singleton generates custom log file after reConfigured") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE(
          "01 Basic Operations",
          "Confirm CECS Singleton generates custom log file after reConfigured"
      )
      //.
      remove("TestConfigCECSSingleton.log");
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually change it's state after
      // Shutdown()
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CHECK_NOTHROW(CECS.reconfigure());
        CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      }

      CECSSingleton::Configuration invalidConfig;
      invalidConfig.loggerName   = "SomeName";
      invalidConfig.logFileName  = "TestConfigCECSSingleton.log";
      invalidConfig.fileLogLevel = 3;
      CECS.setConfiguration(invalidConfig);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_EQ(true, isFileExist(invalidConfig.logFileName));
      CECS.Shutdown();
      int err = remove("TestConfigCECSSingleton.log");
      CHECK_EQ(err, 0);
      std::remove("CECSLog.log");
    }

    TEST_CASE("Test if CECS Singleton Generates or Not the default file based on default config") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE(
          "01 Basic Operations",
          "Test if CECS Singleton Generates or Not the default file based on default config"
      )
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually change it's state after
      // Shutdown()
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECS.reconfigure();
        CECS.state = CECSSingleton::State::INIT;
      }

      // Modify the default config to test different cases.
      auto defaultConfig = CECS.getConfiguration();
      if (defaultConfig.fileLogLevel == 0) {
        CHECK_EQ(false, isFileExist(defaultConfig.logFileName));
      } else if (defaultConfig.fileLogLevel < static_cast<uint8_t>(Logger::L::NONE)) {
        CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
        CHECK_EQ(true, isFileExist(defaultConfig.logFileName));
      }
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }

    TEST_CASE("Test logging with CECS...") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("01 Basic Operations", "Basic logging with CECS...")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECS.reconfigure();
        CECS.state = CECSSingleton::State::INIT;
      }

      auto configuration         = CECS.getConfiguration();
      configuration.fileLogLevel = Logger::L::TRC;
      CECS.setConfiguration(configuration);
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_NOTHROW(CECS.logMsg(Logger::L::TRC, "... Trace Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::DBG, "... Debug Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::INFO, "... Info Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::ERR, "... Error Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::WARN, "... Warning Message ... "));
      _ERRSTR(1, { ss << "_ERRSTR: ... Error Message ... "; })
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }
  }
} // namespace docTests
