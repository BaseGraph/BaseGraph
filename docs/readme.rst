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
- Various common metrics
- Python interface generated with `pybind11`_

Design Rationale
----------------

We chose protected inheritance over public inheritance for the undirected graph classes because it ensures that each function call with a directed graph argument cannot accept the derived class.

We chose the STL list to store edges because most algorithms don't rely on searching and deleting specific edges. By using linked lists, the data structure is lightweight in memory and has constant complexity for adding edges (which is a more common operation).


.. _STL: https://en.wikipedia.org/wiki/Standard_Template_Library
.. _pybind11: https://pybind11.readthedocs.io/en/stable/index.html
