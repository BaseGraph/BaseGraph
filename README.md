# BaseGraph

[![C++ unit tests](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml)
[![Python tests](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml)


[BaseGraph] provides a set of C++ objects and functions to analyze, manipulate and simulate the structure of graphs (or [complex networks]). Most of the core functionalities are available in a Python wrapper created with [pybind11].

BaseGraph was motivated by the need for a *__simple__*, *__fast__* and *__easy to install__* library. It aims to stay *__minimalist__*: by default, only a few classes and algorithms are provided. However, BaseGraph is easily *__extensible__* and allows users to decide which extension to install.

## Requirements

  * A C++14 (or newer) compliant compiler
  * [CMake] 3.16+ for the C++ library
  * Python 3.X for the Python module
  * [pybind11] for the Python module


## Example

```C++
    // Initialization with any container of edges
    BaseGraph::DirectedGraph graph(
            std::list<BaseGraph::Edge> {{0, 1}, {0, 3}, {1, 0}}
    );

    // Vertices in the graph
    for (auto vertex: graph)
        std::cout << vertex << ", ";
    std::cout << std::endl;

    // Edges in the graph
    for (const auto& edge: graph.edges)
        std::cout << edge.first << ", " << edge.second << std::endl;

    // Out neighbours
    for (auto neighbour: graph.getOutEdgesOf(0))
        std::cout << vertex << ", ";
    std::cout << std::endl;
```
Additional examples are provided in the ``example`` directory.


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
It's also possible to install BaseGraph into a custom location by adding the `--prefix` argument
```sh
cmake --install build --prefix "/path/to/custom/install/location"
```
Note that using a custom location will prevent CMake from finding BaseGraph with its default search engine.

In another CMake project, the core BaseGraph library and its extensions can be linked using
```cmake
find_library(BaseGraph)
...
target_link_libraries(Target BaseGraph [BaseGraph_extension ...])
```

### Python library

The Python wrapper __doesn't require__ the regular C++ installation.

First, install the requirements
```sh
pip install pybind11 scikit-build
```
Then, install BaseGraph Python interface with
```sh
pip install ./base_graph
```

## Testing
In order to build the unit tests, the `BUILD_TESTS` flag must be enabled with
```sh
cmake -B build -DBUILD_TESTS=on
```
The option is saved in the CMake cache so it only needs to be set once.

The [GoogleTest] framework is used to manage the unit tests. GoogleTest doesn't need to be installed because it is automatically pulled from GitHub by the CMake build.

## Documentation

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
[complex networks]: https://en.wikipedia.org/wiki/Complex_network
[pybind11]:         https://github.com/pybind/pybind11
[CMake]:            https://cmake.org
[STL]:              https://en.cppreference.com/w
[GoogleTest]:       https://github.com/google/googletest
[Doxygen]:          https://doxygen.nl/
[Sphinx]:           https://www.sphinx-doc.org/en/master/
[Breathe]:          https://breathe.readthedocs.io/en/latest/
[Read the Docs]:    https://readthedocs.org/
[Sy Brand's blog]:  https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake
