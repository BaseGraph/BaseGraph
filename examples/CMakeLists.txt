cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph_cpp_example LANGUAGES CXX)

# Set C++11 requirements
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)

# Locate BaseGraph installation
# If the installation is not system-wide (cmake install using a prefix),
# CMake must be given the prefix <path> given at the installation:
#   cmake --install --prefix "<path>"
# Here we suppose that the command
#   cmake --install --prefix "$HOME/.local"
# was ran.
set(CMAKE_PREFIX_PATH "$ENV{HOME}/.local" ${CMAKE_PREFIX_PATH})

# Import BaseGraph package
find_package(BaseGraph REQUIRED)

# Create example executables
add_executable(directed_example directed_graph.cpp)
add_executable(undirected_example undirected_graph.cpp)
add_executable(edgelabeled_directed_example edgelabeled_directedgraph.cpp)
add_executable(edgelabeled_undirected_example edgelabeled_undirectedgraph.cpp)

# Link executables to BaseGraph core
target_link_libraries(directed_example BaseGraph::core)
target_link_libraries(undirected_example BaseGraph::core)
target_link_libraries(edgelabeled_directed_example BaseGraph::core)
target_link_libraries(edgelabeled_undirected_example BaseGraph::core)
