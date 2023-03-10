Installation
============

To install BaseGraph, clone the git repository with

.. code-block:: console

   $ git clone https://github.com/antoineallard/base_graph.git


.. _cpp-install:

C++ library
-----------

To compile the static library, run

.. code-block:: console

    $ cd base_graph
    $ cmake -B build
    $ cmake --build build

One can use `cmake --build build -jn` to compile in `n` parallel jobs. The static libraries will be placed in `build/lib`.

To install the BaseGraph and link it in other projects, run (may require root/admin privileges)

.. code-block:: console

    $ cmake --install build

It's also possible to install BaseGraph into a custom location by adding the `--prefix` argument. For example, on UNIX-like systems, BaseGraph can be installed in `~/.local`

.. code-block:: console

    $ cmake --install build --prefix "~/.local"

Note that using a custom location will prevent CMake from finding BaseGraph with its default search engine. To allow CMake to find it, append the location to the `PATH` environment variable or or to CMake's search path

.. code-block:: console

    $ cmake --build build -DCMAKE_PREFIX_PATH="~/.local"

Note that CMake's search paths are project-specific, meaning that you will need to set the variable for each external project using BaseGraph. However, because CMake caches variables,the variable only needs to be set once per project.

In another CMake project, the BaseGraph library is linked using

.. code-block:: cmake

    find_library(BaseGraph)
    ...
    target_link_libraries(Target BaseGraph::core)

Python module
--------------

Installing the Python module **does not require** the C++ library.

After installing the requirements

.. code-block:: console

   $ pip install pybind11 scikit-build-core

the BaseGraph Python module is installed with

.. code-block:: console

   $ pip install .
