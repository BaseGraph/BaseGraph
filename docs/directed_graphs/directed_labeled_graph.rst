.. code-block:: cpp

    #include "BaseGraph/directed_graph.hpp"


Labeled directed graph
======================

:ref:`LabeledDirectedGraph <labeleddirectedgraph>` is a class which is able to
store edge information (i.e. a label).

Usage
-----

Edge-labeled directed graphs are created and manipulated very similarly to a
:ref:`DirectedGraph <directedgraph>`. Suppose edges represent flights between
cities

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


Detailed documentation
----------------------

.. _labeleddirectedgraph:

.. doxygenclass:: BaseGraph::LabeledDirectedGraph
    :project: BaseGraph
    :members:
