#ifndef BaseGraph_EDGE_LABELED_GRAPH_PYBIND_H
#define BaseGraph_EDGE_LABELED_GRAPH_PYBIND_H

#include <string>
#include <vector>
#include <sstream>

#include "BaseGraph/types.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace py = pybind11;
using namespace BaseGraph;


template<typename EdgeLabel>
typename std::enable_if<!std::is_integral<EdgeLabel>::value>::type
    declareSpecializedEdgeLabeledDirectedGraph(py::class_<EdgeLabeledDirectedGraph<EdgeLabel>>& pyClass){}

template<typename EdgeLabel>
typename std::enable_if<std::is_integral<EdgeLabel>::value>::type
    declareSpecializedEdgeLabeledDirectedGraph(py::class_<EdgeLabeledDirectedGraph<EdgeLabel>>& pyClass){
        using Class = EdgeLabeledDirectedGraph<EdgeLabel>;

        pyClass
            .def("get_total_edge_number",    [](const Class& self){ return self.getTotalEdgeNumber(); });
    }

template<typename EdgeLabel>
typename std::enable_if<!std::is_integral<EdgeLabel>::value>::type
    declareSpecializedEdgeLabeledUndirectedGraph(py::class_<EdgeLabeledUndirectedGraph<EdgeLabel>>& pyClass){}

template<typename EdgeLabel>
typename std::enable_if<std::is_integral<EdgeLabel>::value>::type
    declareSpecializedEdgeLabeledUndirectedGraph(py::class_<EdgeLabeledUndirectedGraph<EdgeLabel>>& pyClass){
        using Class = EdgeLabeledUndirectedGraph<EdgeLabel>;

        pyClass
            .def("get_total_edge_number",    [](const Class& self){ return self.getTotalEdgeNumber(); });
    }

template<typename EdgeLabel>
void declareEdgeLabeledDirectedGraph(py::module& m, const std::string& typestr) {
    using Class = EdgeLabeledDirectedGraph<EdgeLabel>;
    std::string pyClassName = "EdgeLabeledDirectedGraph"+typestr;

    auto PyClass = py::class_<Class> (m, pyClassName.c_str());
    PyClass
        .def(py::init<size_t>(), py::arg("size"))
        .def("resize",                   &Class::resize, py::arg("size"))
        .def("get_size",                 &Class::getSize)
        .def("get_distinct_edge_number", &Class::getDistinctEdgeNumber)

        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("label"), py::arg("force")=false)
        .def("add_reciprocal_edge_idx", py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addReciprocalEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("label"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&Class::removeEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"))
        .def("is_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&Class::isEdgeIdx, py::const_),
                                          py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist_idx", &Class::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges",       &Class::removeMultiedges)
        .def("remove_selfloops",        &Class::removeSelfLoops)
        .def("clear_edges",             &Class::clearEdges)

        .def("set_edge_label_to", py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&>(&Class::setEdgeLabelTo),
                                    py::arg("source index"), py::arg("destination index"), py::arg("new label"))
        .def("get_edge_label_of", py::overload_cast<VertexIndex, VertexIndex>(&Class::isEdgeIdx, py::const_),
                                    py::arg("source index"), py::arg("destination index"))

        .def("get_out_edges_of_idx",  &Class::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("get_in_edges",          &Class::getInEdges)
        .def("get_adjacency_matrix",  &Class::getAdjacencyMatrix)
        .def("get_in_degree_of_idx",  &Class::getInDegreeOfIdx, py::arg("vertex index"))
        .def("get_in_degrees",        &Class::getInDegrees)
        .def("get_out_degree_of_idx", &Class::getOutDegreeOfIdx, py::arg("vertex index"))
        .def("get_out_degrees",       &Class::getOutDegrees)

        .def("get_deep_copy",        [](const Class& self) { return Class(self); })
        //.def("get_undirected_graph", [](const Class& self) { return EdgeLabeledUndirectedGraph<EdgeLabel>(self); })
        .def("get_reversed_graph",   &Class::getReversedGraph)
        .def("get_subgraph_of_idx",  [](const Class& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                       py::arg("vertices of subgraph"))

        .def("__eq__",      [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
        .def("__getitem__", &Class::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("__str__",     [](const Class self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const Class &self) { return py::make_iterator(self.begin(), self.end()); },
                            py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     &Class::getSize);

    declareSpecializedEdgeLabeledDirectedGraph<EdgeLabel>(PyClass);
}

template<typename EdgeLabel>
void declareEdgeLabeledUndirectedGraph(py::module& m, const std::string& typestr) {
    using Class = EdgeLabeledUndirectedGraph<EdgeLabel>;
    std::string pyClassName = "EdgeLabeledUndirectedGraph"+typestr;

    auto PyClass = py::class_<Class> (m, pyClassName.c_str());
    PyClass
        .def(py::init<size_t>(), py::arg("size"))
        .def("resize",                   &Class::resize, py::arg("size"))
        .def("get_size",                 &Class::getSize)
        .def("get_distinct_edge_number", &Class::getDistinctEdgeNumber)

        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("label"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&Class::removeEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("is_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&Class::isEdgeIdx, py::const_),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist_idx", &Class::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_multiedges",       &Class::removeMultiedges)
        .def("remove_selfloops",        &Class::removeSelfLoops)
        .def("clear_edges",             &Class::clearEdges)

        .def("set_edge_label_to", py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&>(&Class::setEdgeLabelTo),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("new label"))
        .def("get_edge_label_of", py::overload_cast<VertexIndex, VertexIndex>(&Class::isEdgeIdx, py::const_),
                                    py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("get_out_edges_of_idx", &Class::getOutEdgesOfIdx, py::arg("vertex index"))
        .def("get_adjacency_matrix", &Class::getAdjacencyMatrix)
        .def("get_degree_of_idx",    &Class::getDegreeOfIdx, py::arg("vertex index"), py::arg("with self-loops")=true)
        .def("get_degrees",          &Class::getDegrees, py::arg("with self-loops")=true)

        .def("get_deep_copy",        [](const Class& self) { return Class(self); })
        //.def("get_directed_graph", [](const Class& self) { return self.getDirectedGraph(); })
        .def("get_subgraph_of_idx",  [](const Class& self, const std::vector<VertexIndex>& vertices) { return self.getSubgraphOfIdx(vertices.begin(), vertices.end()); },
                                       py::arg("vertices of subgraph"))

        .def("__eq__",      [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
        .def("__getitem__", [](const Class self, VertexIndex idx) { return self.getOutEdgesOfIdx(idx); })
        .def("__str__",     [](const Class self)  { std::ostringstream ret; ret << self; return ret.str(); })
        .def("__iter__",    [](const Class &self) { return py::make_iterator(self.begin(), self.end()); },
                            py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        .def("__len__",     &Class::getSize);

    declareSpecializedEdgeLabeledUndirectedGraph<EdgeLabel>(PyClass);
}

#endif
