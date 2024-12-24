#pragma once
#include <string>
#include <fstream>
#include <doctest.h>

// NOLINTBEGIN
#ifndef __FNAME__
  #define __FNAMEBSL__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
  #define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FNAMEBSL__ )
#endif
// NOLINTEND

struct TestCaseInfo {
  std::string suite;
  std::string name;
  std::string filename;
  size_t lineNumber;
};

#define DOCUMENT_TEST_CASE(suiteName, testName)  addTestCase({suiteName, testName, __FNAME__, __LINE__});


// #define DOCUMENT_TEST_CASE(suiteName, testName)  \
//   DOCTEST_TEST_CASE(testName) \
//   {  \
//     static bool added = []() {  \
//       addTestCase({suiteName, testName, __FILE__, __LINE__});  \
//       return true;  \
//     }();  \
//   }

void addTestCase(const TestCaseInfo & test_case_info);
void exportTestCasesToLog(const std::string& filename);

