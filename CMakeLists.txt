cmake_minimum_required(VERSION 3.30)

project(CECS)
project(${PROJECT_NAME} VERSION 0.1.1)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/API-Headers/version.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h"
  @ONLY
)


set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.0
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(spdlog)
set(EXTERNAL_LIBS ${EXTERNAL_LIBS} spdlog::spdlog)

set(LIB_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/API-Headers)

file(GLOB_RECURSE LIB_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB LIB_HEADER_FILES
  ${LIB_HEADERS_DIR}/*.hpp
)

#message(STATUS "CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
#message(STATUS "LIB_HEADERS_DIR=${LIB_HEADERS_DIR}")
#message(STATUS "LIB_SOURCE_FILES=${LIB_SOURCE_FILES}")
#message(STATUS "LIB_HEADER_FILES=${LIB_HEADER_FILES}")

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_HEADERS_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTERNAL_LIBS})
target_sources(${PROJECT_NAME} PUBLIC ${LIB_HEADER_FILES}) # CLion



# ===========================================================================================
# ===========================================================================================
# Testing
# ===========================================================================================
# ===========================================================================================

set(TARGET_TEST_NAME "CECS_Tests")

set(TEST_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/unitTests/include)

file(GLOB_RECURSE TEST_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/unitTests/*.cpp
)
file(GLOB_RECURSE TEST_HEADER_FILES
  ${TEST_HEADERS_DIR}/*.hpp
)

add_executable(${TARGET_TEST_NAME} ${TEST_SOURCE_FILES})
target_include_directories(${TARGET_TEST_NAME} PUBLIC ${TEST_HEADERS_DIR} ${LIB_HEADERS_DIR})
target_link_libraries(${TARGET_TEST_NAME} PUBLIC ${PROJECT_NAME} ${EXTERNAL_LIBS})
target_sources(${TARGET_TEST_NAME} PUBLIC ${TEST_HEADER_FILES}) # CLion