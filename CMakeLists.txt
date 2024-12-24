cmake_minimum_required(VERSION 3.30)
project(CECS)

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.0
)
FetchContent_MakeAvailable(spdlog)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/API-Headers)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unitTests/include/)

file(GLOB_RECURSE LIB_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE TEST_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/unitTests/*.cpp
)

add_library(CECS SHARED
  ${LIB_SOURCE_FILES}
)
target_link_libraries(CECS PUBLIC spdlog::spdlog)

add_executable(CECS_Tests ${TEST_SOURCE_FILES}
  unitTests/include/TestsDescriptionLogger.hpp
  unitTests/TestsDescriptionLogger.cpp)
target_link_libraries(CECS_Tests PRIVATE CECS)
