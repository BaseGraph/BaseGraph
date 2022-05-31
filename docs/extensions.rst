Extensions
==========

BaseGraph is designed to be extended with all kinds of extensions. These
extensions are provided in the form of git submodules. By default, when
the BaseGraph project is cloned, the extensions submodules won't be expanded
(not available). BaseGraph provides a small program ``bg`` that can be used
to manage the extensions.


Manage extensions
-----------------

To install an extension named ``ext``, use

.. code-block:: console

   python bg extension install ext

.. note::

    The script ``bg`` can used as an executable script. This shortens the command
    to ``./bg command``.

With the same syntax, the extensions can be updated (checkout submodule to most
recent commit), uninstalled and purged

.. code-block:: console

   ./bg extension update extension1 [extension2, ...]
   ./bg extension uninstall extension1 [extension2, ...]
   ./bg extension purge extension1 [extension2, ...]

To list the installed extensions, use

.. code-block:: console

   ./bg extension list

An extension can be added to BaseGraph using

.. code-block:: console

   ./bg extension create ext_name host_url

where ``ext_name`` is the name of the extension and where ``host_url`` is the
url of the hosted repository.


Developping new extensions
--------------------------

If you're familiar with the C++, you might be interested in implementing
additional features to BaseGraph. This section describes how to create
a BaseGraph C++ extension library, Python bindings with pybind and
unit tests.

Header-only extensions
++++++++++++++++++++++

For simple extensions, one can implement functions and classes directly in
header files so that no compilation and linking is required. Let's create
a header-only extension named "header-only-ex" that implements a function
that removes an edge.

Start by creating a repository on a hosting service (e.g. GitHub,
GitLab, etc.). Now add it as an extension to BaseGraph using

.. code-block:: console

   ./bg extension create header-only-ex repo_url

This creates a submodule located at ``include/BaseGraph/extensions/header-only-ex``.
In this new submodule, create the header file ``header_feature.h`` that
implements the new feature

.. code-block:: cpp

   #ifndef BASEGRAPH_EXT_HEADER_ONLY_H
   #define BASEGRAPH_EXT_HEADER_ONLY_H

   #include "BaseGraph/undirectedgraph.h"

   namespace BaseGraph { namespace HeaderOnly {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2) {
        graph.removeEdgeIdx(vertex1, vertex2);
   }

   }} // BaseGraph::HeaderOnly

   #endif

After creating a commit in the extension repository, update the submodule and
reinstall BaseGraph C++ library

.. code-block:: console

   ./bg extension update header-only-ex
   ./bg install cpp

This function is now available with

.. code-block:: cpp

   #include "BaseGraph/extensions/header-only-ex/header_feature.h"
   ...
   BaseGraph::HeaderOnly::removeAnEdge(...);

You can at this point test the new features and create a pull request in the
`BaseGraph repository <https://github.com/antoineallard/base_graph>`_.


Compiled extension
++++++++++++++++++

It's sometimes convenient to compile extensions as libraries to overall reduce
the compile time. However, this requires some work with CMake.

As for the header-only extension, a repository must be created and added
as a BaseGraph extension with

.. code-block:: console

   ./bg extension create compiled-ex repo_url

In the repository, create a header file ``compiled_feature.h``

.. code-block:: cpp

   #ifndef BASEGRAPH_EXT_COMPILED_H
   #define BASEGRAPH_EXT_COMPILED_H

   #include "BaseGraph/undirectedgraph.h"

   namespace BaseGraph { namespace Compiled {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2);

   }} // BaseGraph::Compiled

   #endif


and an implementation file ``compiled_feature.cpp``

.. code-block:: cpp

   #include "compiled_feature.h"

   namespace BaseGraph { namespace Compiled {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2) {
        graph.removeEdgeIdx(vertex1, vertex2);
    }

   }} // BaseGraph::Compiled

Now to integrate the extension into BaseGraph CMake infrastructure, create
the file ``CMakeLists.txt``

.. code-block:: cmake

    # Create library for new extension
    add_library(BaseGraph_compiledfeature compiled_feature.cpp)

    # Instruct BaseGraph to track this extension
    add_extension(BaseGraph_compiledfeature)

Commit these new files to the extension repository, update the extension
in BaseGraph and reinstall the C++ library.

.. code-block:: console

   ./bg extension update
   ./bg install cpp

The new library will now be part of the BaseGraph installation. It will be usable
by linking any executable to ``BaseGraph_compiledfeature`` in another C++ project.
The function is be available with

.. code-block:: cpp

   #include "BaseGraph/extensions/header-only-ex/header_feature.h"
   ...
   BaseGraph::Compiled::removeAnEdge(...);

You can at this point test the new features and create a pull request in the
`BaseGraph repository <https://github.com/antoineallard/base_graph>`_. This example
can be found `here<https://github.com/SILIZ4/BaseGraphExtensionExample>`_.

Python bindings
+++++++++++++++

Unit testing
++++++++++++
