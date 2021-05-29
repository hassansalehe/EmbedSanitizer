cmake_minimum_required(VERSION 3.10)

# set the project name
project(EmbedSanitizer)

enable_testing()

# locate googletest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

## Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

# add the executable
add_executable(race_test race_test.cpp)

# link executable with googletest and pthread library
target_link_libraries(race_test ${GTEST_LIBRARIES} pthread gtest_main)

add_test(test_race race_test)
