## C++ CMake example

Before compiling this example, BaseGraph must be compiled and installed. In the root directory of this repository `base_graph/`,
run
```sh
cmake -B build
cmake --build build
```
Now you can choose to install BaseGraph system-wide (may require root/admin privileges)
```sh
cmake --install build
```
or to a location of your choice, in `your-user/local` directory for example
```sh
cmake --install build --prefix "/home/your-user/local"
```
If a custom installation path is chosen, you must provide it to CMake in the [`CMakeLists.txt`](CMakeLists.txt) of this directory.

Once BaseGraph has been properly installed, this example can be compiled in this directory
```sh
cd example
cmake -B build
cmake --build build
```
The compiled programs will be placed under the `build` directory.
