BaseGraph
=========

**BaseGraph** is an open source C++ library with a Python interface that facilitates manipulations and computations on graphs.

Why another graph library?
--------------------------

While there are many open source graph libraries available, none of them is a simple building block for projects. Based solely on the `STL`_\ , BaseGraph aims to provide simple and efficient graph classes that can be used inside various algorithms.

What does BaseGraph offer?
--------------------------

- Simple base classes such as DirectedGraph and UndirectedGraph
- Highly customizable template classes
- Utilities to read and write utilities in different formats
- Python interface generated with `pybind11`_

Design Rationale
----------------

The undirected graph objects are not (publicly) inherited from the directed graph objects to protect users from passing the wrong type of graph. Internally, the undirected graphs are inherited from directed graphs because a large portion of the code can be reused.

Edges are stored in doubly linked lists because algorithms often only need to iterate on the neighbours of a vertex. In addition to being lightweight in memory, linked list are usually a great data structure for graphs because graphs are often sparse.

.. _STL: https://en.wikipedia.org/wiki/Standard_Template_Library
.. _pybind11: https://pybind11.readthedocs.io/en/stable/index.html
