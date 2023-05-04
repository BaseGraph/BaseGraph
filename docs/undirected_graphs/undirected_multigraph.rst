.. code-block:: cpp

    #include "BaseGraph/undirected_multigraph.hpp"


Undirected multigraphs
======================

:ref:`UndirectedMultigraph <undirectedmultigraph>` is a undirected graph that
allows parallel edges. The number of parallel edges connecting a pair of
vertices is called its *multiplicity*.

Usage
-----

UndirectedMultigraph is used very similarly to :ref:`UndirectedGraph
<undirectedgraph>`, the main difference being that parallel edges are accounted
for when adding and removing edges. For example, we can add an edge
:math:`(i,j)` even if it already exists once. In that case, the multiplicty
will be increased to 2

.. code-block:: cpp

    BaseGraph::UndirectedMultigraph graph(5);

    graph.addEdge(0, 1);
    graph.addEdge(0, 1); // Edge (0, 1) now has multiplicity 2.

The same principle applies when removing an edge: if the multiplicity is 2,
then removing an edge reduces it to 1

.. code-block:: cpp

   graph.removeEdge(0, 1); // Edge (0, 1) now has multiplicity 1.

Additionally, we can add and remove multiple edges at the same time

.. code-block:: cpp

   graph.addMultiedge(0, 2, 3); // Edge (0, 2) has multiplicity 3
   graph.removeMultiedge(0, 2, 2); // Edge (0, 2) has multiplicity 1

The multiplicity of an edge is obtained with ``getEdgeMultiplicity``

.. code-block:: cpp

   std::cout << "Multiplicity of edge (0, 2) is: "
       << graph.getEdgeMultiplicity(0, 2) << std::endl;


Detailed documentation
----------------------

.. _undirectedmultigraph:

.. doxygenclass:: BaseGraph::UndirectedMultigraph
    :project: BaseGraph
    :members:
