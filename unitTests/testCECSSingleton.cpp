#include <CECS.hpp>
#include <TestsDescriptionLogger.hpp>
#include <cstring>

namespace docTests {

  DOCTEST_TEST_SUITE("Basic Operations") {
    TEST_CASE("Test CECS Singleton") {
      addTestCase({"Basic Operations", "Test CECS Singleton", __FNAME__, __LINE__});
      const BaseCECS * ecsPtr = BaseCECS::getInstance();
      CHECK_NE(ecsPtr, nullptr);
      // Ensure that acquisition of instances, generate at any moment in time
      // will generate a default name for the instance "CECS-Default". This can
      // change by one static variable constructor.
      CHECK_EQ(ecsPtr->getECSName(), "CECS-Default");
    }
  }

}
