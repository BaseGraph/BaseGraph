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
manipulate graphs. However, it's possible to build on BaseGraph from
external projects. This section details our suggested approach to do so.

TODO
