cmake_minimum_required(VERSION 3.30)
project(CECS)

set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/API-Headers)
add_library(CECS SHARED src/CECS.cpp)
