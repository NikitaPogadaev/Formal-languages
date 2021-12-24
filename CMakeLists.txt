cmake_minimum_required(VERSION 2.6)

project(KS-grammtics)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(formal formal.cpp)

target_link_libraries(formal ${GTEST_LIBRARY_DEBUG} ${GTEST_MAIN_LIBRARY_DEBUG})

add_test(
NAME formal
COMMAND formal
)
