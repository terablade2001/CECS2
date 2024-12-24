#define DOCTEST_CONFIG_IMPLEMENT
#include <TestsDescriptionLogger.hpp>
#include <iostream>
#include <vector>

// struct TestCasesExporter {
//   ~TestCasesExporter() {
//     exportTestCasesToLog("test_cases_log.txt");
//   }
// } exporter;

static std::vector<TestCaseInfo> testCases{};

void exportTestCasesToLog(const std::string& filename) {
  static std::mutex mutex;
  std::lock_guard<std::mutex> lock(mutex);

  std::ofstream logFile(filename);
  if (!logFile.is_open()) {
    std::cerr << "Error: Unable to open file for writing tests documentation: [" << filename << "]" << std::endl;
    return;
  }
  logFile << "Test Cases Reference List\n";
  logFile << "==================================================\n";
  for (const auto& testCase : testCases) {
    logFile << "["<<testCase.suite<<":: "<<testCase.name<<"]("<<testCase.filename<<": "<<testCase.lineNumber<<")"<<std::endl;
  }
}

void addTestCase(const TestCaseInfo & test_case_info) {
  testCases.push_back(test_case_info);
}


int main(int argc, char** argv) {
  doctest::Context context;
  context.applyCommandLine(argc, argv);
  int res = context.run();
  exportTestCasesToLog("test_cases_log.txt");
  if (context.shouldExit()) { return res;}
  return res;
}