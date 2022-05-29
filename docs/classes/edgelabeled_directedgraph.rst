.. code-block:: cpp

    #include "BaseGraph/edgelabeled_directedgraph.h"

Edge-labeled Directed graphs
============================

EdgeLabeledDirectedGraph is a template subclass of DirectedGraph for which each
directed edge has a label. As DirectedGraph, vertices are identified by an
integer index ranging from :math:`0` to :math:`n-1`, where :math:`n` is the
number of vertices.

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

    BaseGraph::EdgeLabeledDirectedGraph<Flight> graph(5);

To add a flight in this graph, use the method ``addEdgeIdx`` with the
appropriate label

.. code-block:: cpp

    graph.addEdgeIdx(0, 1, {"Company A", 10.});
    graph.addEdgeIdx(4, 3, {"Company B", 2.2});

The information about each flight is retrieved using ``getEdgeLabelOfIdx``

.. code-block:: cpp

    Flight flightA = graph.getEdgeLabelOfIdx(0, 1);
    Flight flightB = graph.getEdgeLabelOfIdx(4, 3);

It's also possible to change the value of a label with ``setEdgeLabelIdx``

.. code-block:: cpp

    graph.setEdgeLabelIdx(0, 1, {"Company B", 10.});

The methods, metrics and algorithms applicable on a DirectedGraph can
also be used with ``graph``

.. code-block:: cpp

   graph.getOutDegreeOfIdx(0); // 1


Methods documentation
---------------------

.. doxygenclass:: BaseGraph::EdgeLabeledDirectedGraph
    :project: BaseGraph
    :members:
