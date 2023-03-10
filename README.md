# BaseGraph

[![C++ unit tests](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml)
[![Python tests](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml)


[BaseGraph] is a C++ library providing basic graph objects, tools to read and write them to files and a few algorithms.
Most of the core functionalities are available in a Python wrapper created with [pybind11].

BaseGraph was motivated by the need for a __simple__, __fast__ and __easy to install__ library. BaseGraph should remain small, but extensions are welcomed in other repositories!

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
    for (const auto& edge: graph.edges)
        std::cout << edge.first << ", " << edge.second << std::endl;

    // Print out neighbours of vertex 0
    for (auto neighbour: graph.getOutEdgesOf(0))
        std::cout << vertex << ", ";
```
Additional examples are provided in the [examples](examples/) directory.


## Installation
Clone this repository
```sh
git clone https://github.com/antoineallard/base_graph.git
```

### C++ library
To compile the static library, run
```sh
cd base_graph
cmake -B build
cmake --build build
```
One can use `cmake --build build -jn` to compile in `n` parallel jobs. The static libraries will be placed in `build/lib`.

To install the BaseGraph and link it in other projects, run (may require root/admin privileges)
```sh
cmake --install build
```
It's also possible to install BaseGraph into a custom location by adding the `--prefix` argument. For example, on UNIX-like systems, BaseGraph can be installed in `~/.local`
```sh
cmake --install build --prefix "~/.local"
```
Note that using a custom location will prevent CMake from finding BaseGraph with its default search engine. To allow CMake to find it, append the location to the `PATH` environment variable or or to CMake's search path
```cs
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

The Python wrapper __doesn't require__ the regular C++ installation.

First, install the requirements
```sh
pip install pybind11 scikit-build-core
```
Then, install BaseGraph Python module with
```sh
pip install ./base_graph
```

## Building tests
In order to build the unit tests, the `BUILD_TESTS` flag must be enabled with
```sh
cmake -B build -DBUILD_TESTS=on
```
The option is saved in the CMake cache so it only needs to be set once.

The unit tests are built in the [GoogleTest] framework. The framework must be installed on the system to compile the tests.

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
