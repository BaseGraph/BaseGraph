.. code-block:: cpp

    #include "BaseGraph/undirected_weighted_graph.hpp"


Undirected weighted graph
=========================

:ref:`UndirectedWeightedGraph <undirectedweightedgraph>` is an undirected graph
in which each edge has a weight.

Usage
-----

UndirectedWeightedGraph is a :ref:`LabeledUndirectedGraph
<labeledundirectedgraph>` where the edge labels are weights. For that reason,
both are almost identical. For example, we add a few weighted edges

.. code-block:: cpp

    BaseGraph::UndirectedWeightedGraph graph(5);

    graph.addEdge(0, 1, -10);
    graph.addEdge(1, 1, 0);
    graph.addEdge(2, 4, 1.5);

We can modify a existing weight

.. code-block:: cpp

    graph.setEdgeWeight(1, 1, 5);

get the weight of any edge

.. code-block:: cpp

    graph.getEdgeWeight(2, 4); // 1.5

and get the sum of the weights in the graph

.. code-block:: cpp

    graph.getTotalWeight(); // -10 + 5 + 1.5 = 3.5


Detailed documentation
----------------------

.. _undirectedweightedgraph:

.. doxygenclass:: BaseGraph::UndirectedWeightedGraph
    :project: BaseGraph
    :members:
