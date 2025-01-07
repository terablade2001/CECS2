#include <CECS.hpp>
#include <TestsDescriptionLogger.hpp>
#include <cstring>

namespace docTests {

  DOCTEST_TEST_SUITE(
      "Basic Operations"
  ) {
    TEST_CASE("Test CECS Singleton") {
      LOG_TEST_CASE("Basic Operations", "CECS has default argument")
      auto &CECS = CECSSingleton::getInstance();
      CHECK_EQ(CECS.getECSName(), "CECS-Default");
      SUBCASE("Changing the CECSSingleton name") {
        CECS.setECSName("Test");
        CHECK_EQ(CECS.getECSName(), "Test");
      }
    }
  }

} // namespace docTests
