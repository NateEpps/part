cmake_minimum_required(VERSION 3.0)

project(part VERSION 0.1)

message(STATUS "Project located at ${CMAKE_SOURCE_DIR}")

# Put output in build dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Write config
configure_file(${CMAKE_SOURCE_DIR}/src/Config.hpp.in ${CMAKE_BINARY_DIR}/Config.hpp)
include_directories(${CMAKE_BINARY_DIR})
message(STATUS "configure_file: Wrote ${CMAKE_BINARY_DIR}/Config.hpp")

# Build
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
