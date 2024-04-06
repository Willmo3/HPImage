cmake_minimum_required(VERSION 3.27)
project(HPImage)

set(CMAKE_CXX_STANDARD 14)

# GoogleTest install:
# GoogleTest requires at least C++14
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Windows compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(hpimage STATIC
        hpimage.h
        hpimage.cpp
)

add_executable(tests test_hpimage.cpp)
target_link_libraries(tests GTest::gtest_main hpimage)
