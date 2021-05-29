cmake_minimum_required(VERSION 3.10)

# set the project name
project(EmbedSanitizer)

enable_testing()

# locate googletest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
