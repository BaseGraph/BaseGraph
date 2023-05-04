.. code-block:: cpp

    #include "BaseGraph/directed_multigraph.hpp"


Directed multigraphs
====================

:ref:`DirectedMultigraph <directedmultigraph>` is a directed graph that allows
parallel edges. The number of parallel edges connecting a pair of vertices is
called its *multiplicity*.

Usage
-----

DirectedMultigraph is used very similarly to :ref:`DirectedGraph
<directedgraph>`, the main difference being that parallel edges are accounted
for when adding and removing edges. For example, we can add an edge
:math:`(i,j)` even if it already exists. In that case, the multiplicty is
increased to 2

.. code-block:: cpp

    BaseGraph::DirectedMultigraph graph(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 1); // Edge (0, 1) now has multiplicity 2.

The same principle applies when removing an edge: if the multiplicity is 2, then removing an edge reduces it to 1

.. code-block:: cpp

   graph.removeEdge(0, 1); // Edge (0, 1) now has multiplicity 1.

In addition, multiple edges can be added or removed at the same time

.. code-block:: cpp

   graph.addMultiedge(0, 2, 3); // Edge (0, 2) has multiplicity 3
   graph.removeMultiedge(0, 2, 2); // Edge (0, 2) has multiplicity 1

The multiplicity of an edge is obtained with ``getEdgeMultiplicity``

.. code-block:: cpp

   std::cout << "Multiplicity of edge (0, 2) is: "
       << graph.getEdgeMultiplicity(0, 2) << std::endl;


Detailed documentation
----------------------

.. _directedmultigraph:

.. doxygenclass:: BaseGraph::DirectedMultigraph
    :project: BaseGraph
    :members:
