#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/directed_multigraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/undirected_multigraph.h"

#include "vertexlabeled_graphs.hpp"
#include "edgelabeled_graphs.hpp"


namespace py = pybind11;
using namespace BaseGraph;


static void defineGraphs(py::module &m);
static void defineEdgelabeledGraphs(py::module &m);
static void defineMultigraphs(py::module &m);
static void defineVertexLabeledGraphs(py::module &core, py::module &io);


void defineAllGraphs(py::module &core, py::module &io) {
    defineGraphs(core);
    defineEdgelabeledGraphs(core);
    defineMultigraphs(core);  // must be defined after edge labeled graphs (inheritance)
    defineVertexLabeledGraphs(core, io);
}


static void defineGraphs(py::module &m) {
    py::class_<DirectedGraph> (m, "DirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def(py::init<std::list<Edge>>(), py::arg("edge list"))

        .def("resize",          &DirectedGraph::resize, py::arg("size"))
        .def("get_size",        &DirectedGraph::getSize)
        .def("get_edge_number", &DirectedGraph::getEdgeNumber)
        .def("is_directed",     [](const UndirectedGraph &self) { return true; })

        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("force")=false)
        .def("add_reciprocal_edge_idx", py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedGraph::addReciprocalEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::removeEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"))
        .def("is_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&DirectedGraph::isEdgeIdx, py::const_),
                                          py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist_idx", &DirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_duplicate_edges",  &DirectedGraph::removeDuplicateEdges)
        .def("remove_self_loops",       &DirectedGraph::removeSelfLoops)
        .def("clear_edges",             &DirectedGraph::clearEdges)

        .def("get_out_edges_of_idx",  &DirectedGraph::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("get_in_edges",          &DirectedGraph::getInEdges)
        .def("get_adjacency_matrix",  &DirectedGraph::getAdjacencyMatrix)
        .def("get_in_degree_of_idx",  &DirectedGraph::getInDegreeOfIdx, py::arg("vertex index"))
        .def("get_in_degrees",        &DirectedGraph::getInDegrees)
        .def("get_out_degree_of_idx", &DirectedGraph::getOutDegreeOfIdx, py::arg("vertex index"))
        .def("get_out_degrees",       &DirectedGraph::getOutDegrees)

        .def("get_deep_copy",        [](const DirectedGraph& self) { return DirectedGraph(self); })
        .def("get_undirected_graph", [](const DirectedGraph& self) { return UndirectedGraph(self); })
        .def("get_reversed_graph",   &DirectedGraph::getReversedGraph)
        .def("get_subgraph_of_idx",  [](const DirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def("__eq__",      [](const DirectedGraph& self, const DirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const DirectedGraph& self, const DirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const DirectedGraph self, VertexIndex idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__str__",     [](const DirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const DirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                            py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const DirectedGraph self)  { return self.getSize(); });


    py::class_<UndirectedGraph> (m, "UndirectedGraph")
        .def(py::init<size_t>(), py::arg("size"))
        .def(py::init<std::list<Edge>>(), py::arg("edge list"))

        .def("resize",          [&](UndirectedGraph& self, size_t size) { self.resize(size); }, py::arg("size"))
        .def("get_size",        [&](UndirectedGraph& self) { return self.getSize(); })
        .def("get_edge_number", [&](UndirectedGraph& self) { return self.getEdgeNumber(); })
        .def("is_directed",     [](const UndirectedGraph &self) { return false; })

        .def("add_edge_idx",      py::overload_cast<VertexIndex, VertexIndex, bool> (&UndirectedGraph::addEdgeIdx),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("force")=false)
        .def("is_edge_idx",       py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::isEdgeIdx, py::const_),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_edge_idx",   py::overload_cast<VertexIndex, VertexIndex>(&UndirectedGraph::removeEdgeIdx),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist_idx", &UndirectedGraph::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_duplicate_edges", &UndirectedGraph::removeDuplicateEdges)
        .def("remove_self_loops", [&](UndirectedGraph& self) { self.removeSelfLoops(); })
        .def("clear_edges",       [&](UndirectedGraph& self) { self.clearEdges(); })

        .def("get_neighbours_of_idx", &UndirectedGraph::getNeighboursOfIdx, py::arg("vertex index"))
        .def("get_out_edges_of_idx",  &UndirectedGraph::getNeighboursOfIdx, py::arg("vertex index"))
        .def("get_adjacency_matrix",  &UndirectedGraph::getAdjacencyMatrix)
        .def("get_degree_of_idx",     &UndirectedGraph::getDegreeOfIdx, py::arg("vertex index"), py::arg("with self-loops")=true)
        .def("get_degrees",           &UndirectedGraph::getDegrees, py::arg("with self-loops")=true)

        .def("get_directed_graph",  &UndirectedGraph::getDirectedGraph)
        .def("get_deep_copy",       [](const UndirectedGraph& self) {return UndirectedGraph(self);})
        .def("get_subgraph_of_idx", [](const UndirectedGraph& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                        py::arg("subgraph vertices"))

        .def("__eq__",      [](const UndirectedGraph& self, const UndirectedGraph& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const UndirectedGraph& self, const UndirectedGraph& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const UndirectedGraph self, VertexIndex idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__str__",     [](const UndirectedGraph self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const UndirectedGraph &self) { return py::make_iterator(self.begin(), self.end()); },
                              py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     [](const UndirectedGraph self)  { return self.getSize(); });

}


static void defineMultigraphs(py::module &m) {

    py::class_<DirectedMultigraph, EdgeLabeledDirectedGraph<EdgeMultiplicity>> (m, "DirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge_idx",              py::overload_cast<VertexIndex, VertexIndex, bool>(&DirectedMultigraph::addEdgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("force")=false)
        .def("remove_edge_idx",           py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::removeEdgeIdx),
                                            py::arg("source"), py::arg("destination"))
        .def("add_multiedge_idx",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&DirectedMultigraph::addMultiedgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge_idx",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::removeMultiedgeIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"))

        .def("get_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex>(&DirectedMultigraph::getEdgeMultiplicityIdx, py::const_),
                                            py::arg("source"), py::arg("destination"))
        .def("set_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&DirectedMultigraph::setEdgeMultiplicityIdx),
                                            py::arg("source"), py::arg("destination"), py::arg("multiplicity"));

    py::class_<UndirectedMultigraph, EdgeLabeledUndirectedGraph<EdgeMultiplicity>> (m, "UndirectedMultigraph")
        .def(py::init<size_t>(), py::arg("size"))

        .def("add_edge_idx",              py::overload_cast<VertexIndex, VertexIndex, bool>(&UndirectedMultigraph::addEdgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("force")=false)
        .def("remove_edge_idx",           py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::removeEdgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("add_multiedge_idx",         py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(&UndirectedMultigraph::addMultiedgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"), py::arg("force")=false)
        .def("remove_multiedge_idx",      py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::removeMultiedgeIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"))

        .def("get_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex>(&UndirectedMultigraph::getEdgeMultiplicityIdx, py::const_),
                                            py::arg("vertex1"), py::arg("vertex2"))
        .def("set_edge_multiplicity_idx", py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(&UndirectedMultigraph::setEdgeMultiplicityIdx),
                                            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"));
}


static void defineEdgelabeledGraphs(py::module &m) {
    defineEdgeLabeledGraphs<EdgeMultiplicity>(m, "UnsignedInt");  // required to compile multigraph classes
}


static void defineVertexLabeledGraphs(py::module &core, py::module &io) {
    defineVertexLabeledGraphs<std::string, true>(core, io, "Str");  // required to compile load functions from text files
    defineVertexLabeledGraphs<int, true>(core, io, "Int");
}
