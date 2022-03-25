cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph LANGUAGES CXX VERSION 1.0.0
        DESCRIPTION "A minimalist library that facilitates the manipulation, the reading and writing and the computation of metrics of graphs.")

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})


option(BUILD_TESTS "build gtest unit tests" off)
option(BUILD_DOCS  "build doxygen documentation" off)


include_directories(include)
add_subdirectory(src)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_DOCS)
    add_subdirectory(docs)
endif()
