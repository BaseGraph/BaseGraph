Extending BaseGraph
===============================

.. _extending_basegraph:

Extending BaseGraph using CMake
-------------------------------

By design, BaseGraph is simple and provides the bare minimum required to
manipulate graphs. However, it's possible to extend BaseGraph from
external projects. This section shows how we believe it should be done.

BaseGraph extensions are **independent projects** that are integrated in
BaseGraph's ecosystem. For that reason, turning a project into an extension
should be simple. A small example extension is available on
`GitHub <https://github.com/SILIZ4/BaseGraphExtensionExample>`_.

"Integrating BaseGraph's ecosystem" means that:

1. The project's headers are located in
   ``include/BaseGraph/extensions/library-name/``.

2. The project's public C++ API is wrapped inside the ``BaseGraph::``
   namespace **and** inside a second namespace named after the extension.
   For example:

.. code-block:: cpp

    // File: include/BaseGraph/extensions/myextension/defs.h

    // Use a precise (and long enough) include guard
    #ifndef BASEGRAPH_EXTENSION_MY_EXTENSION_H
    #define BASEGRAPH_EXTENSION_MY_EXTENSION_H

    namespace BaseGraph {
    namespace myextension {

    // API declarations

    }} // namespace BaseGraph::myextension
    #endif

3. The project's CMake targets are wrapped inside the ``BaseGraph::``
   namespace. This is done when installing the CMake targets. For example:

.. code-block:: cmake

    install(
        EXPORT MyExtension-targets
        NAMESPACE BaseGraph::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/myextension"
    )


Wrapping extensions in Python using pybind11
--------------------------------------------
It's possible to write a Python wrapper for an extension using `pybind11`_ and
integrate it with BaseGraph's own Python module seamlessly. This can be done in
many ways, but we show how it can be done using CMake since the extension
likely already contains CMake build files. We use `scikit-build-core`_ to
install the module as it's the suggested approach by pybind11. The
`example <https://github.com/SILIZ4/BaseGraphExtensionExample>`_ linked earlier
also defines an example Python wrapper module.

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

We then instruct CMake to copy both the ``_mymodule`` Python module and the
``__init__.py`` inside BaseGraph's Python module directory (which is handled by
scikit-build)

.. code-block:: cmake

    install(TARGETS _mymodule DESTINATION ./basegraph/mymodule)
    install(FILES mymodule/__init__.py DESTINATION ./basegraph/mymodule)


.. note::

   The extension's Python module **must** contain a ``__init__.py`` file
   in order to be recognized by ``basegraph``.

In order to install the library with Python utilities such as pip, we create a
``pyproject.toml`` file. For example,

.. code-block:: toml

    [build-system]
    requires = ["scikit-build-core>=0.2.1", "pybind11"]
    build-backend = "scikit_build_core.build"

    [project]
    name = "basegraph_mymodule"
    version = "0.1.0"
    description = "A BaseGraph extension."
    readme = "README.md"
    authors = [
      { name = "You" },
    ]
    requires-python = ">=3.7"

    [tool.scikit-build]
    wheel.expand-macos-universal-tags = true

The module should now be installable using ``pip install .`` and
should automatically be loaded when importing ``basegraph``:

.. code-block:: python

   import basegraph
   # or from basegraph import mymodule

   basegraph.mymodule.do_something()

.. _pybind11: https://pybind11.readthedocs.io/en/stable/index.html
.. _scikit-build-core: https://scikit-build.readthedocs.io/en/latest/
