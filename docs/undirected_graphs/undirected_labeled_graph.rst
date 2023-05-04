.. code-block:: cpp

    #include "BaseGraph/undirected_graph.hpp"


Labeled undirected graph
========================

:ref:`LabeledUndirectedGraph <labeledundirectedgraph>` is a class which is able
to store edge information (i.e. a label).

Usage
-----

Edge-labeled undirected graphs are created and manipulated very similarly to
:ref:`UndirectedGraph <undirectedgraph>`. Suppose edges represent relationships

.. code-block:: cpp

    struct Relationship {
        enum Status {
            MARRIED, DIVORCED
        } status;
        unsigned int marriageDuration;
    };

An undirected graph of these relationships is created using

.. code-block:: cpp

    BaseGraph::LabeledUndirectedGraph<Relationship> graph(5);

To add a relationship in this graph, use ``addEdge`` with the desired label

.. code-block:: cpp

    graph.addEdge(0, 1, {Relationship::Status::MARRIED, 10});
    graph.addEdge(4, 3, {Relationship::Status::DIVORCED, 5});

The information about each relationship is retrieved using ``getEdgeLabel``

.. code-block:: cpp

    Relationship relationshipA = graph.getEdgeLabel(0, 1);
    Relationship relationshipB = graph.getEdgeLabel(4, 3);

It's also possible to change the value of a label with ``setEdgeLabel``

.. code-block:: cpp

    graph.setEdgeLabel(0, 1, {Relationship::Status::DIVORCED, 10});


Detailed documentation
----------------------

.. _labeledundirectedgraph:

.. doxygenclass:: BaseGraph::LabeledUndirectedGraph
    :project: BaseGraph
    :members:
