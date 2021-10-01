cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(PortableGraphLibrary CXX)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


option(BUILD_TESTS "build gtest unit tests" off)


include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
