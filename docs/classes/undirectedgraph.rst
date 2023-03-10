.. code-block:: cpp

    #include "BaseGraph/undirectedgraph.h"

Undirected graphs
=================


UndirectedGraph is the most basic form of undirected graph, a graph in which
each edges have no orientation (:math:`i\rightarrow j` and :math:`j\rightarrow i`
are equivalent). In this class, vertices are identified by an integer index,
:ref:`VertexIndex <vertexindex>`, ranging from :math:`0` to :math:`n-1`, where
:math:`n` is the number of vertices.

Usage
-----

To create a UndirectedGraph of 5 vertices, use

.. code-block:: cpp

    BaseGraph::UndirectedGraph graph(5);

The vertices of this graph are labeled 0, 1, 2, 3, 4.  To add an edge, we use the method ``addEdge``

.. code-block:: cpp

    graph.addEdge(0, 1);
    graph.addEdge(3, 2);

If we try to make an operation on a vertex outside the ``graph`` range, an ``std::out_of_range`` exception is thrown

.. code-block:: cpp

    // graph.addEdge(1, 5); throws std::out_of_range


Here is a small example that computes the degree of a vertex using a BaseGraph::UndirectedGraph

.. literalinclude:: /../examples/undirected_graph.cpp
    :language: cpp
    :linenos:

Methods documentation
---------------------

.. doxygenclass:: BaseGraph::UndirectedGraph
    :project: BaseGraph
    :members:
