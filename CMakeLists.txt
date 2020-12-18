cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(PortableGraphLibrary)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)


option(BUILD_TESTS "build gtest unit tests" off)


include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
