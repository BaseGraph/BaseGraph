#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/directed_multigraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/undirected_multigraph.h"

#include "edgelabeled_graphs.hpp"


namespace py = pybind11;
using namespace BaseGraph;


static void defineGraphs(py::module &m);
static void defineEdgelabeledGraphs(py::module &m);
static void defineMultigraphs(py::module &m);


void defineAllGraphs(py::module &m) {
    defineGraphs(m);
    defineEdgelabeledGraphs(m);
    defineMultigraphs(m);  // must be defined after edge labeled graphs (inheritance)
}


static void defineGraphs(py::module &m) {
    py::class_<DirectedGraph> (m, "DirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def(py::init<std::list<Edge>>(), py::arg("edge list"))

        .def("resize",          &DirectedGraph::resize, py::arg("size"))
        .def("get_size",        &DirectedGraph::getSize)
        .def("get_edge_number", &DirectedGraph::getEdgeNumber)
        .def("is_directed",     [](const UndirectedGraph &self) { return true; })

        .def("add_edge",            py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addEdge),
                                          py::arg("source index"), py::arg("destination index"), py::arg("force")=false)
        .def("add_reciprocal_edge", py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addReciprocalEdge),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("remove_edge",         py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::removeEdge),
                                          py::arg("source index"), py::arg("destination index"))
        .def("has_edge",             py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::hasEdge, py::const_),
                                          py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist", &DirectedGraph::removeVertexFromEdgeList, py::arg("vertex index"))
        .def("remove_duplicate_edges",  &DirectedGraph::removeDuplicateEdges)
        .def("remove_self_loops",       &DirectedGraph::removeSelfLoops)
        .def("clear_edges",             &DirectedGraph::clearEdges)

        .def("get_out_edges_of",  &DirectedGraph::getOutEdgesOf, py::arg("vertex index"))
        .def("get_in_edges",          &DirectedGraph::getInEdges)
        .def("get_edges",             &DirectedGraph::getEdges)
        .def("get_adjacency_matrix",  &DirectedGraph::getAdjacencyMatrix)
        .def("get_in_degree_of",  &DirectedGraph::getInDegreeOf, py::arg("vertex index"))
        .def("get_in_degrees",        &DirectedGraph::getInDegrees)
        .def("get_out_degree_of", &DirectedGraph::getOutDegreeOf, py::arg("vertex index"))
        .def("get_out_degrees",       &DirectedGraph::getOutDegrees)

        .def("get_deep_copy",        [](const DirectedGraph& self) { return DirectedGraph(self); })
        .def("get_undirected_graph", [](const DirectedGraph& self) { return UndirectedGraph(self); })
        .def("get_reversed_graph",   &DirectedGraph::getReversedGraph)
        .def("get_subgraph_of",  [](const DirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOf(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def_readonly("edges", &DirectedGraph::edges)

        .def("__eq__",      [](const DirectedGraph& self, const DirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const DirectedGraph& self, const DirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const DirectedGraph self, VertexIndex idx) { return self.getOutEdgesOf(idx); })
        .def("__str__",     [](const DirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const DirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                            py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const DirectedGraph self)  { return self.getSize(); });

    py::class_<DirectedGraph::Edges>(m, "DirectedEdgeIterator")
        .def("__iter__",    [](const DirectedGraph::Edges &self) { return py::make_iterator(self.begin(), self.end()); });

    py::class_<UndirectedGraph> (m, "UndirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def(py::init<std::list<Edge>>(), py::arg("edge list"))

        .def("resize",          [&](UndirectedGraph& self, size_t size) { self.resize(size); }, py::arg("size"))
        .def("get_size",        [&](UndirectedGraph& self) { return self.getSize(); })
        .def("get_edge_number", [&](UndirectedGraph& self) { return self.getEdgeNumber(); })
        .def("is_directed",     [](const UndirectedGraph &self) { return false; })

        .def("add_edge",      py::overload_cast<VertexIndex, VertexIndex, bool> (&UndirectedGraph::addEdge),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("has_edge",       py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::hasEdge, py::const_),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_edge",   py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::removeEdge),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist", &UndirectedGraph::removeVertexFromEdgeList, py::arg("vertex index"))
        .def("remove_duplicate_edges", &UndirectedGraph::removeDuplicateEdges)
        .def("remove_self_loops", [&](UndirectedGraph& self) { self.removeSelfLoops(); })
        .def("clear_edges",       [&](UndirectedGraph& self) { self.clearEdges(); })

        .def("get_neighbours_of", &UndirectedGraph::getNeighboursOf, py::arg("vertex index"))
        .def("get_out_edges_of",  &UndirectedGraph::getNeighboursOf, py::arg("vertex index"))
        .def("get_edges",             &UndirectedGraph::getEdges)
        .def("get_adjacency_matrix",  &UndirectedGraph::getAdjacencyMatrix)
        .def("get_degree_of",     &UndirectedGraph::getDegreeOf, py::arg("vertex index"), py::arg("with self-loops")=true)
        .def("get_degrees",           &UndirectedGraph::getDegrees, py::arg("with self-loops")=true)

        .def("get_directed_graph",  &UndirectedGraph::getDirectedGraph)
        .def("get_deep_copy",       [](const UndirectedGraph& self) {return UndirectedGraph(self);})
        .def("get_subgraph_of", [](const UndirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOf(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def_readonly("edges", &UndirectedGraph::edges)

        .def("__eq__",      [](const UndirectedGraph& self, const UndirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const UndirectedGraph& self, const UndirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const UndirectedGraph self, VertexIndex idx) { return self.getOutEdgesOf(idx); })
        .def("__str__",     [](const UndirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const UndirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                              py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const UndirectedGraph self)  { return self.getSize(); });

    py::class_<UndirectedGraph::Edges>(m, "UndirectedEdgeIterator")
        .def("__iter__",    [](const UndirectedGraph::Edges &self) { return py::make_iterator(self.begin(), self.end()); });
}


static void defineMultigraphs(py::module &m) {

    py::class_<DirectedMultigraph, EdgeLabeledDirectedGraph<EdgeMultiplicity>> (m, "DirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge",              py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedMultigraph::addEdge),
                                            py::arg("source"), py::arg("destination"), py::arg("force")=false)
        .def("remove_edge",           py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::removeEdge),
                                            py::arg("source"), py::arg("destination"))
        .def("add_multiedge",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&DirectedMultigraph::addMultiedge),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::removeMultiedge),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"))

        .def("get_edge_multiplicity", py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::getEdgeMultiplicity, py::const_),
                                            py::arg("source"), py::arg("destination"))
        .def("set_edge_multiplicity", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::setEdgeMultiplicity),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"));

    py::class_<UndirectedMultigraph, EdgeLabeledUndirectedGraph<EdgeMultiplicity>> (m, "UndirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge",              py::overload_cast<VertexIndex, VertexIndex, bool>(&UndirectedMultigraph::addEdge),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("force")=false)
        .def("remove_edge",           py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::removeEdge),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("add_multiedge",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&UndirectedMultigraph::addMultiedge),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::removeMultiedge),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"))

        .def("get_edge_multiplicity", py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::getEdgeMultiplicity, py::const_),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("set_edge_multiplicity", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::setEdgeMultiplicity),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"));
}


static void defineEdgelabeledGraphs(py::module &m) {
    defineEdgeLabeledGraphs<EdgeMultiplicity>(m, "UnsignedInt");  // required to compile multigraph classes
}
