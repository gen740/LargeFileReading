cmake_minimum_required(VERSION 3.28)

project(large_file CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

# Setting Build Type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Enable C++ Warnings
set(CMAKE_CXX_FLAGS
    "-Wall -Wextra -Werror -Wnon-virtual-dtor -Wfloat-equal -Winline")

include(FetchContent)
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_Declare(
  googlebench
  URL https://github.com/google/benchmark/archive/refs/tags/v1.8.3.tar.gz
      CMAKE_ARGS -DBENCHMARK_ENABLE_TESTING=OFF)
FetchContent_MakeAvailable(googlebench)

add_library(std_module)
target_sources(std_module PUBLIC FILE_SET CXX_MODULES FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/StdModules.cc)

add_executable(main main.cc)
target_link_libraries(main benchmark::benchmark std_module)
