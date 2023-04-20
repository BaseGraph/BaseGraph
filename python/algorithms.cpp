#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/algorithms/paths.hpp"
#include "BaseGraph/algorithms/topology.hpp"
#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/undirected_graph.hpp"

namespace py = pybind11;
using namespace BaseGraph;

template <template <class...> class Graph, typename EdgeLabel>
void defineAlgorithmsType(py::module &m) {
    using Class = Graph<EdgeLabel>;
    m.def("find_geodesics", &algorithms::findGeodesics<Graph, EdgeLabel>);
    m.def("find_all_geodesics",
          &algorithms::findAllGeodesics<Graph, EdgeLabel>);
    m.def("find_geodesics_from_vertex",
          &algorithms::findGeodesicsFromVertex<Graph, EdgeLabel>);
    m.def("find_all_geodesics_from_vertex",
          &algorithms::findAllGeodesicsFromVertex<Graph, EdgeLabel>);
    m.def("find_subgraph", &algorithms::getSubgraph<Graph, EdgeLabel>);
    m.def("find_subgraph_with_remap",
          &algorithms::getSubgraphWithRemap<Graph, EdgeLabel>);
}

template <typename EdgeLabel>
void defineAlgorithmsDirectedUndirected(py::module &m) {
    defineAlgorithmsType<LabeledDirectedGraph, EdgeLabel>(m);
    defineAlgorithmsType<LabeledUndirectedGraph, EdgeLabel>(m);
}

void defineAlgorithms(py::module &m) {
    defineAlgorithmsDirectedUndirected<NoLabel>(m);
}
