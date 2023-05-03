.. code-block:: cpp

    #include "BaseGraph/undirected_graph.hpp"

Undirected graphs
=================

Undirected graphs contain edges that have no orientation (:math:`i\rightarrow
j` and :math:`j\rightarrow i` are equivalent). Vertices are
identified by an integer index, :ref:`VertexIndex <vertexindex>`, ranging from
:math:`0` to :math:`n-1`, where :math:`n` is the number of vertices.

Unlabeled undirected graph
--------------------------

:ref:`UndirectedGraph <undirectedgraph>` class is a special case of
:ref:`LabeledUndirectedGraph <labeledundirectedgraph>` that has no edge labels.
They share most of their methods.

To create a :ref:`UndirectedGraph <undirectedgraph>` of 5 vertices, use

.. code-block:: cpp

    BaseGraph::UndirectedGraph graph(5);

The vertices of this graph are labeled 0, 1, 2, 3, 4.  To add an edge, we use
the method ``addEdge``

.. code-block:: cpp

    graph.addEdge(0, 1);
    graph.addEdge(3, 2);

If we try to make an operation on a vertex outside the ``graph`` range, an
``std::out_of_range`` exception is thrown

.. code-block:: cpp

    // graph.addEdge(1, 5); throws std::out_of_range


Here is a small example that computes the degree of a vertex

.. literalinclude:: /../examples/undirected_graph.cpp
    :language: cpp
    :linenos:

Labeled undirected graph
------------------------

:ref:`LabeledUndirectedGraph <labeledundirectedgraph>` is a class which is able
to store edge information (i.e. a label).

Edge-labeled undirected graphs are created and manipulated very similarly to
undirected graphs. Suppose edges represent relationship of couples

.. code-block:: cpp

    struct Relationship {
        enum Status {
            MARRIED, DIVORCED
        } status;
        unsigned int durationInYears;
    };

A undirected graph of these relashionships is created using

.. code-block:: cpp

    BaseGraph::LabeledUndirectedGraph<Relationship> graph(5);

To add a relationship in this graph, use ``addEdge`` with the desired label

.. code-block:: cpp

    graph.addEdge(0, 1, {Relationship::Status::MARRIED, 10});
    graph.addEdge(4, 3, {Relationship::Status::DIVORCED, 5});

The information about each relashion is retrieved using ``getEdgeLabel``

.. code-block:: cpp

    Relationship relationA = graph.getEdgeLabel(0, 1);
    Relationship relationB = graph.getEdgeLabel(4, 3);

It's also possible to change the value of a label with ``setEdgeLabel``

.. code-block:: cpp

    graph.setEdgeLabel(0, 1, {Relationship::Status::DIVORCED, 10});


Detailed documentation
----------------------

.. _undirectedgraph:

.. doxygentypedef:: BaseGraph::UndirectedGraph
    :project: BaseGraph

.. _labeledundirectedgraph:

.. doxygenclass:: BaseGraph::LabeledUndirectedGraph
    :project: BaseGraph
    :members:
