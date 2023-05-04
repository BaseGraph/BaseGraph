.. code-block:: cpp

    #include "BaseGraph/undirected_graph.hpp"


Undirected graph (unlabeled)
============================

:ref:`UndirectedGraph <undirectedgraph>` class is a special case of
:ref:`LabeledUndirectedGraph <labeledundirectedgraph>` that has no edge labels.
They share most of their methods.

Usage
-----

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


Detailed documentation
----------------------

See :ref:`LabeledUndirectedGraph <labeledundirectedgraph>`.

.. _undirectedgraph:

.. doxygentypedef:: BaseGraph::UndirectedGraph
    :project: BaseGraph
