Installation
============

To install BaseGraph, clone the git repository with

.. code-block:: console

   $ git clone https://github.com/antoineallard/base_graph.git


.. _cpp-install:

Installing C++ library
----------------------

Being header-only, there is no need to compile BaseGraph. However, CMake can be
used to install the library on your system (may require root/admin privileges)

.. code-block:: console

    $ cd base_graph
    $ cmake -B build
    $ cmake --install build

BaseGraph can also be installed into a custom location by adding the `--prefix`
argument. For example, on UNIX-like systems, one can install it in `~/.local`

.. code-block:: console

    $ cmake --install build --prefix "~/.local"

Note that using a custom location prevents CMake from finding BaseGraph with
its default search engine. To allow CMake to find it, append the location to
your system's `PATH` environment variable or to CMake's search path

.. code-block:: console

    $ cmake --build build -DCMAKE_PREFIX_PATH="~/.local"

Note that on the one hand, CMake's search path is project-specific, meaning
that you will need to set the variable for each external project that uses
BaseGraph. On the other hand, because CMake caches variables,the variable only
needs to be set once per project.

In another CMake project, link BaseGraph with

.. code-block:: cmake

    find_library(BaseGraph)
    ...
    target_link_libraries(<target> BaseGraph::core)

Installing the Python wrapper
-----------------------------

Installing the Python module **does not require** installing the C++ library.

First install the requirements

.. code-block:: console

   pip install -r requirements.txt

the BaseGraph Python module is installed with

.. code-block:: console

   $ pip install ./base_graph
