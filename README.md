# BaseGraph

[![C++ unit tests](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/cpp_unit_tests.yml)
[![Python tests](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml/badge.svg)](https://github.com/antoineallard/base_graph/actions/workflows/compare_networkx.yml)


BaseGraph provides a set of C++ objects and functions to analyze, manipulate and simulate the structure of graphs (or [complex networks]).

Most of the core functionalities of the C++ library are available as a Python module. Objects and functions are wrapped using [pybind11].

## Requirements

  * A C++14 (or newer) compliant compiler
  * [CMake] 3.16+ for the C++ library
  * Python 3.X for the Python module
  * [pybind11] for the Python module


## Why another graph library?

BaseGraph was motivated by the need of an __expressive__ and __easy to install__ library that remains __fast__ and __light-weight__. It aims to facilitate the development of efficient graph algorithms on C++ using only the [STL]. A Python interface is also provided to make scripting with BaseGraph easier.

## Installation
Clone this repository
```sh
git clone https://github.com/antoineallard/base_graph.git
```

### Python library

If pybind11 isn't installed, run
```sh
pip install pybind11
```
Install BaseGraph Python interface with
```sh
pip install ./base_graph
```

### C++ core library
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
# Append BaseGraph path to CMake
set(CMAKE_PREFIX_PATH "/path/to/repository/build" ${CMAKE_MODULE_PATH})
find_library(BaseGraph)
...
target_link_libraries(Target BaseGraph [BaseGraph_extension ...])
```
A few small examples of BaseGraph usage are provided in the ``example`` directory.

## Testing
In order to build the unit tests, the `BUILD_TESTS` flag must be enabled with
```sh
cmake -B build -DBUILD_TESTS=on ..
```
The option is saved in the CMake cache so it only needs to be set once.

The [GoogleTest] framework is used to manage the unit tests. GoogleTest doesn't need to be installed because it is automatically pulled from GitHub by the CMake build.

## Documentation

Special thanks to [Sy Brand's blog] which helped setting up Sphinx with Doxygen using Breathe.


[complex networks]: https://en.wikipedia.org/wiki/Complex_network
[pybind11]:         https://github.com/pybind/pybind11
[CMake]:            https://cmake.org
[STL]:              https://en.cppreference.com/w
[GoogleTest]:       https://github.com/google/googletest
[Sy Brand's blog]:  https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake
