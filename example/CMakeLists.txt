cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(BaseGraph_cpp_example)
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(../include)

add_executable(undirected_example undirected_graph.cpp)
add_executable(vertex_labeled_example vertex_labeled.cpp)

target_link_libraries(undirected_example ${CMAKE_SOURCE_DIR}/../build/src/libBaseGraph.a)
target_link_libraries(vertex_labeled_example ${CMAKE_SOURCE_DIR}/../build/src/libBaseGraph.a)
