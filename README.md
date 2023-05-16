# BaseGraph

[![C++ tests](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml)
[![Python build](https://github.com/antoineallard/base_graph/actions/workflows/python_build.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/python_build.yml)


[BaseGraph] **BaseGraph** is an open source C++ library with a Python interface that makes
handling graphs simple.

## Why should I use BaseGraph

Existing graph C/C++ libraries are either large (long compile time) and/or
difficult to install and/or difficult to use. BaseGraph strives to solve these
issues by:

- being **easy to install**;
- providing a **minimal** yet sufficient set of features;
- offering an **expressive syntax** with a good performance;
- seamlessly integrating **external extensions** developped by the community.


## Dependencies

For the C++ library:
  * A C++14 (or newer) compliant compiler
  * [CMake] 3.16+

For the Python module:
  * Python 3.7+
  * scikit-build-core
  * [pybind11]

## Example

```C++
    // Initialization with any container of edges
    BaseGraph::DirectedGraph graph(
        std::list<BaseGraph::Edge> {{0, 1}, {0, 3}, {1, 0}}
    );

    // Print vertices in the graph
    for (auto vertex: graph)
        std::cout << vertex << ", ";

    // Print edges in the graph
    for (const auto& edge: graph.edges())
        std::cout << edge.first << ", " << edge.second << std::endl;

    // Print out neighbours of vertex 0
    for (auto neighbour: graph.getOutNeighbours(0))
        std::cout << vertex << ", ";
```
Additional examples are provided in the [examples](examples/) directory.


## Installation
Clone this repository
```sh
git clone https://github.com/antoineallard/base_graph.git
```

### C++ library
BaseGraph is a header-only library, meaning that it can be used by copying the header files. However, a CMake build system is also provided to ease the installation. Installing the C++ library is generally required for extensions
```sh
cd base_graph
cmake -B build
# may require root/admin privileges
cmake --install build
```
BaseGraph can be installed in a custom location by adding the `--prefix` argument. For example, on UNIX-like systems, BaseGraph can be installed in `~/.local`
```sh
cmake --install build --prefix "~/.local"
```
Note that using a custom location will prevent CMake from finding BaseGraph with its default search engine. To allow CMake to find it, append the location to the `PATH` environment variable or or to CMake's search path
```sh
cmake --build build -DCMAKE_PREFIX_PATH="~/.local"
```
Note that CMake's search paths are project-specific, meaning that you will need to set the variable for each external project using BaseGraph. However, because CMake caches variables,the variable only needs to be set once per project.

In another CMake project, the BaseGraph library is linked using
```cmake
find_library(BaseGraph)
...
target_link_libraries(Target BaseGraph::core)
```

### Python library

The Python wrapper __doesn't require__ the C++ installation.

First, install the requirements
```sh
cd base_graph
pip install -r requirements.txt
```
Then, install BaseGraph Python module with
```sh
pip install .
```

## Building tests
In order to build the unit tests, the `BUILD_TESTS` flag must be enabled in the CMake configuration with
```sh
cmake -B build -DBUILD_TESTS=on
```
The option is saved in the CMake cache so it only needs to be set once. The tests are built and ran with
```sh
cmake --build build
ctest --verbose
```
Depending on the GMake generator used, compiling in parallel is enable using the flag `-jn` where `n`is the number of cores.
The unit tests use the [GoogleTest] framework. The framework is installed if it is not available on the system.

## Building documentation

The documentation is generated via [Doxygen]+[Breathe]+[Sphinx] and is hosted by [Read the Docs]. To compile the documentation locally, Doxygen must be installed on the system. Sphinx and Read the Docs can be installed through pip
```sh
pip install sphinx_rtd_theme breathe
```
To activate the compilation of the documentation, activate the flag `BUILD_DOCS`
```sh
cmake -B build -DBUILD_DOCS=on
```
The documentation will then be built along the library
```sh
cmake --build build
```

Special thanks to [Sy Brand's blog] which greatly helped setting up Sphinx with Doxygen using Breathe.


[BaseGraph]:        https://base-graph.readthedocs.io/en/latest/
[pybind11]:         https://github.com/pybind/pybind11
[CMake]:            https://cmake.org
[STL]:              https://en.cppreference.com/w
[GoogleTest]:       https://github.com/google/googletest
[Doxygen]:          https://doxygen.nl/
[Sphinx]:           https://www.sphinx-doc.org/en/master/
[Breathe]:          https://breathe.readthedocs.io/en/latest/
[Read the Docs]:    https://readthedocs.org/
[Sy Brand's blog]:  https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake
