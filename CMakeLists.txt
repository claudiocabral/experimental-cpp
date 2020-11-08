cmake_minimum_required(VERSION 3.13)

project(experimental_cpp LANGUAGES CXX VERSION 0.0.1)

add_subdirectory(Catch2)
include(CTest)
include(Catch2/contrib/Catch.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(TEST_BIN tests)

set(TEST_SRCS
  tests/main.cpp
  tests/zip/zip.cpp
  )

set(TEST_INCLUDES
    includes
    )

set(PROPERTIES
    PROPERTIES CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    )

add_executable(${TEST_BIN} ${TEST_SRCS})
set_target_properties(${TEST_BIN} ${PROPERTIES})
target_include_directories(${TEST_BIN} PRIVATE ${TEST_INCLUDES})
target_link_libraries(${TEST_BIN} Catch2::Catch2)
