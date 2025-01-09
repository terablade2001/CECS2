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
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      CHECK_EQ(CECS.getProjectName(), "CECS-Default");
      SUBCASE("Changing the CECSSingleton name") {
        CECS.setProjectName("Test");
        CHECK_EQ(CECS.getProjectName(), "Test");
      }
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }

    TEST_CASE("Configure CECS Singleton with no name should throw") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE("Basic Operations", "Configure CECS Singleton with no name should throw")
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually change it's state after
      // Shutdown()
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECSSingleton::Configuration defaultConfig;
        CECS.setConfiguration(defaultConfig);
        CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      }

      CECSSingleton::Configuration invalidConfig;
      invalidConfig.loggerName = "";
      CHECK_THROWS_AS(CECS.setConfiguration(invalidConfig), std::invalid_argument);
      CECS.Shutdown();
      std::remove("CECSLog.log");
    }

    TEST_CASE("Confirm CECS Singleton generates custom log file after reConfigured") {
      std::lock_guard<std::mutex> lock(testMutex);
      LOG_TEST_CASE(
          "Basic Operations", "Confirm CECS Singleton generates custom log file after reConfigured"
      )
      //.
      remove("TestConfigCECSSingleton.log");
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually change it's state after
      // Shutdown()
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECSSingleton::Configuration defaultConfig;
        CECS.setConfiguration(defaultConfig);
        CHECK_EQ(CECS.state, CECSSingleton::State::INIT);
      }

      CECSSingleton::Configuration invalidConfig;
      invalidConfig.loggerName   = "SomeName";
      invalidConfig.logFileName  = "TestConfigCECSSingleton.log";
      invalidConfig.fileLogLevel = 3;
      CHECK_NOTHROW(CECS.setConfiguration(invalidConfig));
      CHECK_EQ(true, isFileExist(invalidConfig.logFileName));
      CECS.Shutdown();
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
      auto &CECS = CECSSingleton::getInstance();
      // CECS in persistent among all texts, thus we have to manually change it's state after
      // Shutdown()
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      if (CECS.state == CECSSingleton::State::NOT_INIT) {
        CECSSingleton::Configuration defaultConfig;
        CECS.setConfiguration(defaultConfig);
        CECS.state = CECSSingleton::State::INIT;
      }

      // Modify the default config to test different cases.
      auto &defaultConfig = CECS.configuration;
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
      LOG_TEST_CASE("Basic Operations", "Basic logging with CECS...")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.state, CECSSingleton::State::NOT_INIT);
      CECS.configuration.logCustomFormat = "[%^%L%$] %v";
      CECS.reconfigure();

      CECS.configuration.fileLogLevel = Logger::L::TRC;
      CHECK_NOTHROW(CECS.reconfigure());
      CHECK_NOTHROW(CECS.logMsg(Logger::L::TRC, "... Trace Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::DBG, "... Debug Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::INFO, "... Info Message ... "));
      CHECK_NOTHROW(CECS.logMsg(Logger::L::WARN, "... Warning Message ..."));
      {
        std::ostringstream captured_stdout;
        std::streambuf    *original_cout_buffer = std::cout.rdbuf();
        std::cout.rdbuf(captured_stdout.rdbuf());
        std::ostringstream captured_stderr;
        std::streambuf    *original_cerr_buffer = std::cerr.rdbuf();
        std::cerr.rdbuf(captured_stderr.rdbuf());
        CHECK_NOTHROW(CECS.logMsg(Logger::L::ERR, "... Error Message ..."));
        std::cout.rdbuf(original_cout_buffer);
        //      ostringstream oss;
        //        oss << "[E] ... Error Message ... \n[W] ... Warning Message ... ";
        // cout << "[" << oss.str() << "]" << endl;
        cout << "[" << captured_stdout.str() << "]" << endl;
        cout << "[" << captured_stderr.str() << "]" << endl;
        // CHECK_EQ(0, captured_stdout.str().compare(oss.str()));
      }

      CECS.Shutdown();
      std::remove("CECSLog.log");
    }
  }
} // namespace docTests
