cmake_minimum_required(VERSION 3.30)
project(CECS)

#set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
#if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
#  message(STATUS "*** ENABLING COVERAGE ***")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g --coverage -ftest-coverage -fprofile-arcs")
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g --coverage -ftest-coverage -fprofile-arcs")
#  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -ftest-coverage -fprofile-arcs")
#  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage -ftest-coverage -fprofile-arcs")
#endif()

set(CMAKE_CXX_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.0
)
FetchContent_MakeAvailable(spdlog)
set(EXTLIBS ${EXTLIBS} spdlog::spdlog)

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
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTLIBS})
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
target_link_libraries(${TARGET_TEST_NAME} PUBLIC ${PROJECT_NAME} ${EXTLIBS})
target_sources(${TARGET_TEST_NAME} PUBLIC ${TEST_HEADER_FILES}) # CLion