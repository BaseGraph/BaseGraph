# BaseGraph

![Tests](https://github.com/charlesmurphy1/fast-midynet/actions/workflows/run_tests.yml/badge.svg)

BaseGraph provides a set of C++ objects and functions to analyze, manipulate and simulate the structure of graphs (or [complex networks]).

Most of the core functionalities of the C++ library are available as a Python module. Objects and functions are wrapped using [pybind11].

## Why another graph library?

The development of this library was motivated by the need of a __fast__, __lightweight__ and __customizable__ code, which was __easy to install__ and straightforward to deploy on __supercomputing facilities__ whose softwares are not always up-to-date.

## Requirements

  * A C++11 (or newer) compliant compiler
  * [CMake] 3.16+ for the C++ library
  * Python 3.X for the Python module
  * [pybind11] for the Python module


## Installation
Clone this repository
```
git clone https://github.com/antoineallard/base_graph.git
```

#### C++
To compile the static library, run
```sh
cd base_graph
mkdir build
cd build
cmake ..
make
```
The static library will appear in `base_graph/build/libBaseGraph.a` (the extension will vary depending on the operating system). Note: one can use `make -jn` to compile in parallel with n threads (faster).

To link the library in another CMake project, use
```cmake
set(CMAKE_PREFIX_PATH "/path/to/repository/build" ${CMAKE_MODULE_PATH})
...
target_link_libraries(<TARGET> BaseGraph)
```
Make sure to compile BaseGraph before compiling a new project which depends on it.

A few examples of BaseGraph usage are provided in the ``example`` directory.

#### Python library
After cloning the repository, execute
```sh
cd base_graph
pip install .
```

## Testing
In order to build the unit tests, the option `BUILD_TESTS` must be toggled on
```
cmake -DBUILD_TESTS=on ..
```
The option is saved in the CMake cache so it only needs to be set once.

The [GoogleTest] framework is used to manage the unit tests. GoogleTest doesn't need to be installed because it is automatically pulled from GitHub by the CMake build.

[Boost]:            https://www.boost.org
[CMake]:            https://cmake.org
[complex networks]: https://en.wikipedia.org/wiki/Complex_network
[Eigen]:            http://eigen.tuxfamily.org
[graph-tool]:       https://graph-tool.skewed.de/
[igraph]:           https://igraph.org/
[NetworkX]:         https://networkx.github.io/
[pybind11]:         https://github.com/pybind/pybind11
[Spectra]:          https://spectralib.org/
[STL]:              https://en.cppreference.com/w
[GoogleTest]:      https://github.com/google/googletest
