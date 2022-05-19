## C++ CMake example

Before compiling this example, BaseGraph must be compiled. In the root directory of this repository `base_graph/`,
run
```sh
cmake -B build
cmake --build build
```
The CMake example is then compiled using these same commands, but in this `example/` directory
```sh
cd example
cmake -B build
cmake --build build
```
The compiled program will be placed under the `build` directory.
