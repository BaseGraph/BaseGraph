cmake_minimum_required(VERSION 3.15)
project(BaseGraphBenchmarks)

find_package(BaseGraph  REQUIRED)
find_package(BaseGraphRandom REQUIRED)
find_package(Boost REQUIRED)
find_package(igraph REQUIRED)

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark BaseGraph::core BaseGraph::random igraph)
