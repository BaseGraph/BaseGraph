Linking and extending BaseGraph core
====================================

This page explains how to link BaseGraph libraries and how to add
functionalities from an external CMake project.

Linking BaseGraph core in external CMake project
------------------------------------------------

If not done already, install BaseGraph as described in the :ref:`Installation
section <cpp-install>`. In the external CMake project, obtain the core
BaseGraph library include directories and targets using

.. code-block:: console

    find_package(BaseGraph)

and link the core library to the targets

.. code-block:: console

    target_link_libraries(TARGET BaseGraph::core)


Extending BaseGraph using CMake
-------------------------------

By design, BaseGraph is simple and provides the bare minimum required to
manipulate graphs. However, it's possible to extend BaseGraph from
external projects. This section how it should be done (as a standard).

BaseGraph extensions are **independent projects** that are integrated in
BaseGraph's ecosystem. For that reason, turning a project into an extension
should be simple. A small example extension is available on
[GitHub](https://github.com/SILIZ4/BaseGraphExtensionExample).

"Integrating BaseGraph's ecosystem" only means that the library's headers
are placed under ``BaseGraph/extensions/library-name`` and that it uses the
``BaseGraph::`` namespace both in CMake and in C++.

1. The project's headers should all be placed in
   ``include/BaseGraph/extensions/library-name`` so that internal ``#include``
   directive are valid and that the headers are installed at the correct
   location.

2. The project's public C++ API should be wrapped inside the ``BaseGraph::``
   namespace **and** inside a second namespace named after the extension.
   For example:

.. code-block:: cpp

    // include/BaseGraph/extension/myextension/defs.h

    #ifndef BASEGRAPH_EXTENSION_MY_EXTENSION_H
    #define BASEGRAPH_EXTENSION_MY_EXTENSION_H

    namespace BaseGraph { namespace myextension {

    // Some constants functions and classes
    // ...

    }} // namespace BaseGraph::myextension
    #endif

3. The project's CMake targets should be wrapped inside the ``BaseGraph::``
   extension. This is done when installing the CMake targets. For example:

.. code-block:: cmake

    install(
        EXPORT MyExtension-targets
        NAMESPACE BaseGraph::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/myextension"
    )


Wrapping extension in Python using pybind11
-------------------------------------------
It's possible to write a Python wrapper for an extension using `pybind11`_ and
integrate it with BaseGraph's own Python module seamlessly. This can be done in
many ways, but we show how it can be done using CMake since the extension
should already contain a CMake structure. We use ``scikit-build-core`` to
install the module as it's the suggested approach by pybind11. The
[example](https://github.com/SILIZ4/BaseGraphExtensionExample) linked earlier
provides a functionnal Python module.

The pybind11 module itself is written with the usual CMake commands, i.e.

.. code-block:: cmake

   pybind11_add_module(_mymodule mymodule_bindings.cpp)
   target_link_libraries(_mymodule PRIVATE myextension BaseGraph::core)

.. note::

    When writing Python modules for extensions, the `basegraph` module
    must be imported for the objects to be valid. This can be done with

    .. code-block:: cpp

        pybind11::module::import("basegraph");

    inside a ``PYBIND11_MODULE`` macro.

Using ``scikit-build-core`` to build and install the Python module, we add

.. code-block:: cmake

    install(TARGETS _mymodule DESTINATION ./basegraph/mymodule)
    install(FILES mymodule/__init__.py DESTINATION ./basegraph/mymodule)

to place both the ``_mymodule`` Python module and the ``__init__.py`` inside
BaseGraph's Python module directory.

.. note::

   The extension's Python module **must** contain a ``__init__.py`` file
   in order to be recognized by ``basegraph``.

To make the CMake project installable with ``scikit-build-core``, we need to
create a ``pyproject.toml`` file. For example, we can use

.. code-block:: toml

    [build-system]
    requires = ["scikit-build-core>=0.2.1", "pybind11"]
    build-backend = "scikit_build_core.build"

    [project]
    name = "basegraph_mymodule"
    version = "0.1.0"
    description = "My BaseGraph extension module."
    readme = "README.md"
    authors = [
      { name = "You" },
    ]
    requires-python = ">=3.7"

    [tool.scikit-build]
    wheel.expand-macos-universal-tags = true

See their documentation for more examples.

The module should now be installable using ``pip install .``. After the installation,
the module should automatically be loaded when importing ``basegraph``:

.. code-block:: python

   import basegraph
   # or from basegraph import mymodule

   basegraph.mymodule.do_something()

.. _pybind11: https://pybind11.readthedocs.io/en/stable/index.html
