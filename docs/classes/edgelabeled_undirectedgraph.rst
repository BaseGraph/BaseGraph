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


Methods documentation
---------------------

.. doxygenclass:: BaseGraph::EdgeLabeledUndirectedGraph
    :project: BaseGraph
    :members:
