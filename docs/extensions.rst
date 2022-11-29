Extensions
==========

BaseGraph is designed to be extended with all kinds of extensions. These
extensions are provided in the form of git submodules. By default, when
the BaseGraph project is cloned, the extensions submodules won't be expanded
(not available). BaseGraph provides a utility program ``bg`` which
manages the extensions.


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

   ./bg extension create ext_name repo_url

where ``ext_name`` is the name of the extension and where ``repo_url`` is the
url of the hosted Git repository.


Developping new extensions
--------------------------

If you're familiar with C++, you might be interested in implementing
additional features to BaseGraph. This section describes how to create
a BaseGraph C++ extension library, Python bindings with pybind and
unit tests.

Header-only extensions
++++++++++++++++++++++

For simple extensions, one can implement functions and classes directly in
header files to avoid dealing with compilation and linking. Let's create
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

   // include/BaseGraph/extensions/header-only-ex/header_feature.h
   #ifndef BASEGRAPH_EXT_HEADER_ONLY_H
   #define BASEGRAPH_EXT_HEADER_ONLY_H

   #include "BaseGraph/undirectedgraph.h"

   namespace BaseGraph { namespace HeaderOnly {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2) {
        graph.removeEdge(vertex1, vertex2);
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

It's sometimes convenient to compile extensions as libraries. However, this
requires some work with CMake. The following example can be found on GitHub
`here <https://github.com/SILIZ4/BaseGraphExtensionExample>`_.

As for the header-only extension, a repository must be created online and
then added as a BaseGraph extension with

.. code-block:: console

   ./bg extension create compiled-ex repo_url

where "compiled-ex" is the name of our new extension. In the repository,
create a header file ``compiled_feature.h``

.. code-block:: cpp

   // include/BaseGraph/extensions/compiled-ex/compiled_feature.h
   #ifndef BASEGRAPH_EXT_COMPILED_H
   #define BASEGRAPH_EXT_COMPILED_H

   #include "BaseGraph/undirectedgraph.h"

   namespace BaseGraph { namespace Compiled {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2);

   }} // BaseGraph::Compiled

   #endif


and an implementation file ``compiled_feature.cpp``

.. code-block:: cpp

   // include/BaseGraph/extensions/compiled-ex/compiled_feature.cpp
   #include "compiled_feature.h"

   namespace BaseGraph { namespace Compiled {

   void removeAnEdge(BaseGraph::UndirectedGraph& graph, BaseGraph::VertexIndex vertex1, BaseGraph::VertexIndex vertex2) {
        graph.removeEdge(vertex1, vertex2);
    }

   }} // BaseGraph::Compiled

Now to integrate the extension into BaseGraph CMake infrastructure, create
the file ``CMakeLists.txt``

.. code-block:: cmake

    # include/BaseGraph/extensions/compiled-ex/CMakeLists.txt
    # Create library for new extension
    add_library(compiledFeature compiled_feature.cpp)

    # Instruct BaseGraph to track this extension
    add_extension(compiledFeature)

Commit these new files to the extension repository, update the extension
in BaseGraph and reinstall the C++ library.

.. code-block:: console

   ./bg extension update
   ./bg install cpp

The new library will now be part of the BaseGraph installation. It will be usable
by linking any executable to ``BaseGraph::compiledFeature`` in another C++ project
with

.. code-block:: cmake

   # Project CMake code
   target_link_libraries(ProjectTarget BaseGraph::compiledFeature)

where ``ProjectTarget`` is the target in the external project. The feature is
now available in this external project

.. code-block:: cpp

   #include "BaseGraph/extensions/compiled-ex/compiled_feature.h"
   ...
   BaseGraph::Compiled::removeAnEdge(...);

You can at this point test the new features and create a pull request in the
`BaseGraph repository <https://github.com/antoineallard/base_graph>`_.

Providing Python bindings
+++++++++++++++++++++++++

Unit testing
++++++++++++
