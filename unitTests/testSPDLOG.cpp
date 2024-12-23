#include <doctest.h>
#include <spdlog/spdlog.h>

namespace docTests {
  DOCTEST_TEST_SUITE("SPDLOG Tests") {
    TEST_CASE("Test Basic SPDLOG") {
      // Log a message
      spdlog::info("Hello, spdlog!");
      spdlog::warn("This is a warning!");
      // Log with formatting
      spdlog::error("An error occurred: {}", 404);
    }
  }
  DOCTEST_TEST_SUITE_END();
}