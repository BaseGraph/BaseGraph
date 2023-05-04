.. code-block:: cpp

    #include "BaseGraph/directed_weighted_graph.hpp"


Directed weighted graph
=======================

:ref:`DirectedWeightedGraph <directedweightedgraph>` is a directed graph in
which each edge has a weight.

Usage
-----

DirectedWeightedGraph is a :ref:`LabeledDirectedGraph <labeleddirectedgraph>`
where the edge labels are weights. For that reason, both are almost identical.
For example, we add a few weighted edges

.. code-block:: cpp

    BaseGraph::DirectedWeightedGraph graph(5);

    graph.addEdge(0, 1, -10);
    graph.addEdge(1, 0, 0);
    graph.addEdge(3, 0, 1.5);

We can modify a existing weight

.. code-block:: cpp

    graph.setEdgeWeight(1, 0, 5);

get the weight of any edge

.. code-block:: cpp

    graph.getEdgeWeight(3, 0); // 1.5


and get the sum of the weights in the graph

.. code-block:: cpp

    graph.getTotalWeight(); // -10 + 5 + 1.5 = 3.5


Detailed documentation
----------------------

.. _directedweightedgraph:

.. doxygenclass:: BaseGraph::DirectedWeightedGraph
    :project: BaseGraph
    :members:
