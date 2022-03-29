cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph_cpp_example LANGUAGES CXX)

# Set C++11 requirements
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Indicate to CMake where is BaseGraphConfig.cmake located
set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/../build" ${CMAKE_MODULE_PATH})

# Import BaseGraph package
find_package(BaseGraph REQUIRED)

# Compile example executables
add_executable(undirected_example undirected_graph.cpp)
add_executable(vertex_labeled_example vertex_labeled.cpp)

# Link executables to BaseGraph library
target_link_libraries(undirected_example BaseGraph)
target_link_libraries(vertex_labeled_example BaseGraph)
