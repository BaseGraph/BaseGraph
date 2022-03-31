.. code-block:: cpp

    #include "BaseGraph/directedgraph.h"

Directed graphs
===============

DirectedGraph is the most basic form of directed graph, a graph in which each edges have an orientation (:math:`i\rightarrow j` and :math:`j\rightarrow i` are different). In this class, vertices are identified by an integer index, :ref:`VertexIndex <vertexindex>`, ranging from :math:`0` to :math:`n-1`, where :math:`n` is the number of vertices.


Example
-------

To create a DirectedGraph of 5 vertices, use

.. code-block:: cpp

    BaseGraph::DirectedGraph graph(5);

This created ``graph`` vertices are labeled 0, 1, 2, 3, 4.  To add an edge, we use the method ``addEdgeIdx``

.. code-block:: cpp

    graph.addEdgeIdx(0, 1);
    graph.addEdgeIdx(3, 2);

If we try to make an operation on a vertex outside the ``graph`` range, the class throws an ``std::out_of_range`` exception

.. code-block:: cpp

    // graph.addEdgeIdx(1, 5); throws std::out_of_range


Here is a small complete example to show how to compute the out degree of a vertex and the reciprocity (see :ref:`directed metrics <directedmetrics>`) of a directed graph

.. literalinclude:: /../example/directed_graph.cpp
    :language: cpp
    :linenos:


Methods documentation
---------------------


.. doxygenclass:: BaseGraph::DirectedGraph
    :project: BaseGraph
    :members:
