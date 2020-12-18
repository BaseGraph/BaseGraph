cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(PGL_cpp_example)
set(CXX_STANDARD 11)

include_directories(../include)

add_executable(cpp_example main.cpp)
target_link_libraries(cpp_example ${CMAKE_SOURCE_DIR}/../build/src_cpp/libPGL.a)
