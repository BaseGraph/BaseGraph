.. code-block:: cpp

    #include "BaseGraph/directed_graph.hpp"


Directed graphs
===============

DirectedGraph is the most basic form of directed graph, a graph in which each
edge has an orientation (:math:`i\rightarrow j` and :math:`j\rightarrow i` are
different). Vertices are identified by an integer index,
:ref:`VertexIndex <vertexindex>`, ranging from :math:`0` to :math:`n-1`, where
:math:`n` is the number of vertices.

The DirectedGraph class is a special case of LabeledDirectedGraph that has no
edge labels. As such, they share most of their methods.

Usage
-----

To create a DirectedGraph of 5 vertices, use

.. code-block:: cpp

    BaseGraph::DirectedGraph graph(5);

The vertices of this graph are labeled 0, 1, 2, 3, 4.  To add an edge, we use the method ``addEdge``

.. code-block:: cpp

    graph.addEdge(0, 1);
    graph.addEdge(3, 2);

If we try to make an operation on a vertex outside the ``graph`` range, an ``std::out_of_range`` exception is thrown

.. code-block:: cpp

    // graph.addEdge(1, 5); throws std::out_of_range


Here is a small example that computes the out degree of a vertex using a BaseGraph::DirectedGraph

.. literalinclude:: /../examples/directed_graph.cpp
    :language: cpp
    :linenos:


Labeled Directed graphs
============================

BaseGraph::LabeledDirectedGraph is a class which is able to store edge
information (i.e. a label).

Usage
-----

Edge-labeled directed graphs are created and manipulated very similarly to
directed graphs. Suppose edges represent flights between cities

.. code-block:: cpp

    struct Flight {
        std::string company;
        double distance;
    };

A directed graph of these flights is created using

.. code-block:: cpp

    BaseGraph::LabeledDirectedGraph<Flight> graph(5);

To add a flight in this graph, use the method ``addEdge`` with the
appropriate label

.. code-block:: cpp

    graph.addEdge(0, 1, {"Company A", 10.});
    graph.addEdge(4, 3, {"Company B", 2.2});

The information about each flight is retrieved using ``getEdgeLabel``

.. code-block:: cpp

    Flight flightB = graph.getEdgeLabel(4, 3);
    Flight flightA = graph.getEdgeLabel(0, 1);

It's also possible to change the value of a label with ``setEdgeLabel``

.. code-block:: cpp

    graph.setEdgeLabel(0, 1, {"Company B", 10.});

The methods, metrics and algorithms applicable on a DirectedGraph can
also be used with ``graph``

.. code-block:: cpp

   graph.getOutDegreeOf(0); // 1


Methods documentation
---------------------

.. doxygenclass:: BaseGraph::LabeledDirectedGraph
    :project: BaseGraph
    :members:
