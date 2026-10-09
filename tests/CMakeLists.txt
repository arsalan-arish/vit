# CMake rules for building the Google Test suite for the list implementation

find_package(GTest QUIET)
if (NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)
endif()

add_executable(gtest_list
  gtest_list_tests.cpp
  mock_list.c
)

target_include_directories(gtest_list PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
if (TARGET GTest::gtest_main)
  target_link_libraries(gtest_list PRIVATE GTest::gtest_main)
elseif (TARGET gtest_main)
  target_link_libraries(gtest_list PRIVATE gtest_main)
else()
  message(FATAL_ERROR "GoogleTest not found and could not be fetched")
endif()

target_compile_features(gtest_list PRIVATE cxx_std_11)

enable_testing()
add_test(NAME gtest_list COMMAND gtest_list)
