Directed graphs
===============

.. code-block:: cpp

    #include "BaseGraph/directedgraph.h"

Usage
-----

To create a DirectedGraph of 5 vertices, it's as simple as this

.. code-block:: cpp

    BaseGraph::DirectedGraph graph(5);

Vertices in a DirectedGraph are identified by a :ref:`VertexIndex <vertexindex>`, an integer type. This means that the 5 vertices in ``graph`` are named 0, 1, 2, 3, 4.
To add an edge, we use the method ``addEdgeIdx``

.. code-block:: cpp

    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(3, 2);

If we try to make an operation on a vertex outside the ``graph`` range, the class throws an ``std::out_of_range`` exception

.. code-block:: cpp

    // graph.addEdgeIdx(1, 5); throws std::out_of_range


Here is a small complete example to show how to compute the out degree of a vertex and the reciprocity of a directed graph

.. literalinclude:: /../example/directed_graph.cpp
    :language: cpp
    :linenos:


Methods documentation
---------------------

.. doxygenclass:: BaseGraph::DirectedGraph
    :project: BaseGraph
    :members:
