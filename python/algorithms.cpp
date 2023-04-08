#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/algorithms/graphpaths.h"
#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"

namespace py = pybind11;
using namespace BaseGraph;

void defineAlgorithms(py::module &m) {
    // Path algorithms
    m.def("find_geodesics",
          py::overload_cast<const DirectedGraph &, VertexIndex, VertexIndex>(
              &algorithms::findGeodesics<DirectedGraph>));
    m.def("find_geodesics",
          py::overload_cast<const UndirectedGraph &, VertexIndex, VertexIndex>(
              &algorithms::findGeodesics<UndirectedGraph>));
    m.def("find_all_geodesics",
          py::overload_cast<const DirectedGraph &, VertexIndex, VertexIndex>(
              &algorithms::findAllGeodesics<DirectedGraph>));
    m.def("find_all_geodesics",
          py::overload_cast<const UndirectedGraph &, VertexIndex, VertexIndex>(
              &algorithms::findAllGeodesics<UndirectedGraph>));
    m.def("find_geodesics_from_vertex",
          py::overload_cast<const DirectedGraph &, VertexIndex>(
              &algorithms::findGeodesicsFromVertex<DirectedGraph>));
    m.def("find_geodesics_from_vertex",
          py::overload_cast<const UndirectedGraph &, VertexIndex>(
              &algorithms::findGeodesicsFromVertex<UndirectedGraph>));
    m.def("find_all_geodesics_from_vertex",
          py::overload_cast<const DirectedGraph &, VertexIndex>(
              &algorithms::findAllGeodesicsFromVertex<DirectedGraph>));
    m.def("find_all_geodesics_from_vertex",
          py::overload_cast<const UndirectedGraph &, VertexIndex>(
              &algorithms::findAllGeodesicsFromVertex<UndirectedGraph>));
}
