cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(BaseGraph_cpp_example LANGUAGES CXX)

# Set C++11 requirements
set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Locate BaseGraph installation
# If the installation is not system-wide (cmake install using a prefix),
# CMake must be given the path to the different components of BaseGraph.
# These paths are relative to the <path> prefix given at the installation:
#   cmake --install --prefix "<path>"
# Here we suppose that the command
#   cmake --install --prefix "/path/to/repository/local_install"
# was ran. Note that the subdirectories (/lib, /include) are platform dependent.
set(BASEGRAPH_INSTALL_PATH "${CMAKE_SOURCE_DIR}/..")
set(CMAKE_PREFIX_PATH "${BASEGRAPH_INSTALL_PATH}/lib/cmake" ${CMAKE_MODULE_PATH})
set(CMAKE_LIBRARY_PATH "${BASEGRAPH_INSTALL_PATH}/lib" ${CMAKE_LIBRARY_PATH})
include_directories("${BASEGRAPH_INSTALL_PATH}/include")

# Import BaseGraph package
find_package(BaseGraph REQUIRED)

# Compile example executables
add_executable(directed_example directed_graph.cpp)
add_executable(undirected_example undirected_graph.cpp)
add_executable(edgelabeled_directed_example edgelabeled_directedgraph.cpp)
add_executable(edgelabeled_undirected_example edgelabeled_undirectedgraph.cpp)
add_executable(vertex_labeled_example vertex_labeled.cpp)

# Link executables to BaseGraph library
target_link_libraries(directed_example BaseGraph_core)
target_link_libraries(undirected_example BaseGraph_core)
target_link_libraries(edgelabeled_directed_example BaseGraph_core)
target_link_libraries(edgelabeled_undirected_example BaseGraph_core)
target_link_libraries(vertex_labeled_example BaseGraph_core)
