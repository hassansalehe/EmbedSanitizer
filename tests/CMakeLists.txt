cmake_minimum_required(VERSION 3.10)

# Set project name
project(EmbedSanitizer)

enable_testing()

# Add GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

## Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
add_compile_options(-fprofile-arcs -ftest-coverage)

# Add executables
add_executable(race_test race_test.cpp)

# Link executables with GoogleTest and pthread library
target_link_libraries(race_test ${GTEST_LIBRARIES} pthread gtest_main)

# Add tests for Ctest
add_test(test_race race_test)
