#include <CECS.hpp>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>


namespace docTests {
  DOCTEST_TEST_SUITE("Basic Math Operations") {
    TEST_CASE("Test CECS Singleton") {
      const BaseCECS * ecsPtr = BaseCECS::getInstance("testECS");
      CHECK_NE(ecsPtr, nullptr);
      CHECK_EQ(ecsPtr->getECSName(), "testECS");
    }
  }
  DOCTEST_TEST_SUITE_END();
}