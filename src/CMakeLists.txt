cmake_minimum_required(VERSION 3.12.0)

project(ray)

get_filename_component(include_folder ./include ABSOLUTE)

add_library(
  ray
  SHARED
  ${include_folder}/vec.h
  ${include_folder}/ray.h
  ray.cpp
)

target_include_directories(
  ray
  PRIVATE ${include_folder}
)

include(GoogleTest)
include(CTest)

add_executable(
  rayTest
  test/main.cpp
  test/vecTest.cpp
  test/rayTest.cpp
  test/hittableTest.cpp
  test/metalTest.cpp
)
target_include_directories(
  rayTest
  PRIVATE ${include_folder}
  PRIVATE ${gtest_SOURCE_DIR}/include
)
target_link_libraries(rayTest gtest_main)

gtest_discover_tests(
  rayTest
  TEST_LIST tests
)