cmake_minimum_required(VERSION 3.10)

# Set project name
project(EmbedSanitizer)

# Add GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

## Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_compile_options(-g -O0 -Wall -fprofile-arcs -ftest-coverage)
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

# Add executables
add_executable(race_test race_test.cpp)

# Link executables with GoogleTest and pthread library
target_link_libraries(race_test ${GTEST_LIBRARIES} pthread gtest_main)
target_link_libraries(race_test gcov --coverage)

# Add tests for Ctest
add_test(test_race race_test)
