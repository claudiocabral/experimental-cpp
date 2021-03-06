cmake_minimum_required(VERSION 3.13)

project(experimental_cpp LANGUAGES CXX VERSION 0.0.1)

add_subdirectory(Catch2)
include(CTest)
include(Catch2/contrib/Catch.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_BIN tests)

set(TEST_SRCS
  tests/main.cpp
  tests/match.cpp
  tests/zip/zip.cpp
  tests/zip/benchmarks.cpp
  tests/flat_tree/flat_tree.cpp
  )

set(TEST_INCLUDES
    includes
    )

set(PROPERTIES
    PROPERTIES CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )

set(COMPILE_OPTIONS
    -Werror
    -Wall
    -Wextra
    )

add_executable(${TEST_BIN} ${TEST_SRCS})
set_target_properties(${TEST_BIN} ${PROPERTIES})
target_include_directories(${TEST_BIN} PRIVATE ${TEST_INCLUDES})
target_link_libraries(${TEST_BIN} Catch2::Catch2)
target_compile_options(${TEST_BIN} PUBLIC ${COMPILE_OPTIONS})
