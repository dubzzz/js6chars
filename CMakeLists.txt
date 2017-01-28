cmake_minimum_required(VERSION 2.8.8)

project(js6chars C CXX)

message(STATUS "CMake version   : " ${CMAKE_SYSTEM_VERSION})
message(STATUS "Compiler        : " ${CMAKE_CXX_COMPILER})
message(STATUS "Operating System: " ${CMAKE_SYSTEM})
set(CMAKE_CXX_STANDARD 14)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pthread")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
endif()

if(CMAKE_VERSION VERSION_EQUAL 3.1 OR CMAKE_VERSION VERSION_GREATER 3.1)
  set(CMAKE_CXX_STANDARD 14)
else()
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
  CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)
  if (COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (COMPILER_SUPPORTS_CXX1Y)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
  endif()
endif()

file(GLOB SRCS *.cpp *.hpp)
add_executable(js6chars ${SRCS})

