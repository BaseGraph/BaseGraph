BaseGraph
=========

**BaseGraph** is an open source C++ library with a Python interface that makes
handling graphs simple.

Why should I use BaseGraph
--------------------------

Existing graph C/C++ libraries are either large (long compile time) and/or
difficult to install and/or difficult to use. BaseGraph strives to solve these
issues by:

- being **easy to install**;
- providing a **minimal** yet sufficient set of features;
- offering an **expressive syntax** with a good performance;
- seamlessly integrating **external extensions** developped by the community.

What does BaseGraph offer?
--------------------------

- Customizable classe templates
- Utilities to read and write graphs in binary and text format
- Python interface generated with `pybind11`_
- A few standard graph algorithms such as breadth-first search and Dijkstra's algorithm

BaseGraph is designed to be extensible to accomodate the need for more specific
and specialized algorithms. A guide for extension is provided
:ref:`here <extending_basegraph>`.


Design Rationale
----------------

While some graph classes inherit from others to reduce code duplication, the
inheritance is hidden in order to protect users from passing wrong graph types.
We find that different types of graph often require different implementations.
Nonetheless, the API similarity between the classes allows the user to write
code that accepts any type of graph using templates.

Internally, BaseGraph uses adjacency lists to store edges. This is because many
graph algorithms only need to iterate on the neighbours of vertices. Graphs are
also usually sparse, meaning that most vertices have a small degree. As such,
the cost of traversing all the neighbours of a vertex is small when it is
required. In this context, the linked list seems to best fulfill the
requirements.


.. _STL: https://en.wikipedia.org/wiki/Standard_Template_Library
.. _pybind11: https://pybind11.readthedocs.io/en/stable/index.html
