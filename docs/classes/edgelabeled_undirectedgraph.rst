.. code-block:: cpp

    #include "BaseGraph/edgelabeled_undirectedgraph.h"

Edge-labeled Undirected graphs
==============================

EdgeLabeledUndirectedGraph is a template subclass of UndirectedGraph for which
each edge has a label. As UndirectedGraph, vertices are identified by an
integer index ranging from :math:`0` to :math:`n-1`, where :math:`n` is the
number of vertices.

Usage
-----

Edge-labeled directed graphs are created and manipulated very similarly to
directed graphs. Suppose edges represent relationship of couples

.. code-block:: cpp

    struct Relationship {
        enum Status {
            MARRIED, DIVORCED
        } status;
        unsigned int durationInYears;
    };

A undirected graph of these relashionships is created using

.. code-block:: cpp

    BaseGraph::EdgeLabeledUndirectedGraph<Relationship> graph(5);

To add a relationship in this graph, use the method ``addEdge`` with the
appropriate label

.. code-block:: cpp

    graph.addEdge(0, 1, {Relationship::Status::MARRIED, 10});
    graph.addEdge(4, 3, {Relationship::Status::DIVORCED, 5});

The information about each relashion is retrieved using ``getEdgeLabelOf``

.. code-block:: cpp

    Relationship relationA = graph.getEdgeLabelOf(0, 1);
    Relationship relationB = graph.getEdgeLabelOf(4, 3);

It's also possible to change the value of a label with ``setEdgeLabel``

.. code-block:: cpp

    graph.setEdgeLabel(0, 1, {Relationship::Status::DIVORCED, 10});

The methods, metrics and algorithms applicable on a UndirectedGraph can
also be used with ``graph``

.. code-block:: cpp

   graph.getDegreeOf(0); // 1




Methods documentation
---------------------

.. doxygenclass:: BaseGraph::EdgeLabeledUndirectedGraph
    :project: BaseGraph
    :members:
