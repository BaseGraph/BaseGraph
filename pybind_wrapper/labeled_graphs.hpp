#ifndef BaseGraph_LABELED_GRAPH_PYBIND_H
#define BaseGraph_LABELED_GRAPH_PYBIND_H

#include <string>
#include <vector>
#include <sstream>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/directedgraph.h"
#include "BaseGraph/undirectedgraph.h"
#include "BaseGraph/vertexlabeled_graph.hpp"

#include "BaseGraph/fileio.h"
#include "BaseGraph/metrics/directed.h"
#include "BaseGraph/metrics/general.h"
#include "BaseGraph/metrics/undirected.h"
#include "BaseGraph/algorithms/graphpaths.h"
#include "BaseGraph/algorithms/percolation.h"
#include "BaseGraph/algorithms/randomgraphs.h"


namespace py = pybind11;
using namespace BaseGraph;

template<typename Label, bool isHashable>
void declare_undirectedgraph(py::module &m, const std::string &typestr) {
    using Class = VertexLabeledUndirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledUndirectedGraph") + typestr;
    py::class_<Class, UndirectedGraph>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
    .def(py::init<>())
    .def_static("from_base_class", [](const UndirectedGraph& graph, std::vector<Label> vertices) {return Class(graph, vertices);})

    .def("is_vertex", &Class::isVertex, py::arg("vertex label"))
    .def("find_vertex_index", [](Class& self, Label v){ return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex", [](Class& self, Label v, bool force){ self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &Class::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("change_vertex_label_to", [](Class& self, Label v1, Label v2){ self.changeVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices", &Class::getVertices)

    .def("add_edge", &Class::addEdge, py::arg("vertex1 label"), py::arg("vertex2 label"), py::arg("force")=false)
    .def("is_edge", &Class::isEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))
    .def("remove_edge", &Class::removeEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))

    .def("get_neighbours_of", [](Class& self, Label v){ return self.getOutEdgesOf(v); }, py::arg("vertex label"))
    .def("get_degree", [](Class& self, Label v){ return self.getDegree(v); }, py::arg("vertex label"))

    .def("__eq__", [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const Class& self) { std::ostringstream ret; ret << self; return ret.str(); });

    //.def("write_edgelist_in_text_file", py::overload_cast<const Class&, const std::string&>(&writeEdgeListInTextFile<Label>))
    //.def("write_edgelist_in_text_file", py::overload_cast<const Class&, std::ofstream&>(&writeEdgeListInTextFile<Label>))
    //.def("write_edgelist_in_binary_file", py::overload_cast<const Class&, const std::string&, size_t>(&writeEdgeListInBinaryFile<Label>))
    //.def("write_edgelist_in_binary_file", py::overload_cast<const Class&, std::ofstream&, size_t>(&writeEdgeListInBinaryFile<Label>))
    //.def("load_undirected_edgelist_from_binary_file", py::overload_cast<const std::string&, size_t>(&loadUndirectedEdgeListFromBinaryFile<Label>))
    //.def("load_undirected_edgelist_from_binary_file", py::overload_cast<std::ifstream&, size_t>(&loadUndirectedEdgeListFromBinaryFile<Label>))

    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label>))

    //.def("write_vertices_in_binary_file", py::overload_cast<const Class&, const std::string&, size_t>(&writeVerticesInBinaryFile<Label>))
    //.def("write_vertices_in_binary_file", py::overload_cast<const Class&, std::ofstream&, size_t>(&writeVerticesInBinaryFile<Label>))

    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, const std::string&, size_t>(&addVerticesFromBinaryFile<Label>))
    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label>));
}

template<typename Label, bool isHashable>
void declare_directedgraph(py::module &m, const std::string &typestr) {
    using Class = VertexLabeledDirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledDirectedGraph") + typestr;
    py::class_<Class, DirectedGraph>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr())
    .def(py::init<>())
    .def_static("from_base_class", [](const DirectedGraph& graph, std::vector<Label> vertices) {return Class(graph, vertices);})

    .def("is_vertex", &Class::isVertex, py::arg("vertex label"))
    .def("find_vertex_index", [](Class& self, Label v){ return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex", [](Class& self, Label v, bool force){ self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &Class::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("change_vertex_label_to", [](Class& self, Label v1, Label v2){ self.changeVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices", &Class::getVertices)

    .def("add_edge", &Class::addEdge, py::arg("source label"), py::arg("destination label"), py::arg("force")=false)
    .def("is_edge", &Class::isEdge, py::arg("source label"), py::arg("destination label"))
    .def("remove_edge", &Class::removeEdge, py::arg("source label"), py::arg("destination label"))

    .def("get_out_edges_of", [](Class& self, Label v){ return self.getOutEdgesOf(v); }, py::arg("vertex label"))
    .def("get_in_degree", [](Class& self, Label v){ return self.getInDegreeIdx(self.findVertexIndex(v)); }, py::arg("vertex label"))
    .def("get_out_degree", [](Class& self, Label v){ return self.getOutDegree(v); }, py::arg("vertex label"))

    .def("convert_index_list_to_labels", &Class::convertIndicesListToLabels)
    .def("convert_index_vector_to_labels", &Class::convertIndicesVectorToLabels)

    .def("__eq__", [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const Class& self) { std::ostringstream ret; ret << self; return ret.str(); });

    //.def("write_edgelist_in_text_file", py::overload_cast<const Class&, const std::string&>(&writeEdgeListInTextFile<Label>))
    //.def("write_edgelist_in_text_file", py::overload_cast<const Class&, std::ofstream&>(&writeEdgeListInTextFile<Label>))
    //.def("write_edgelist_in_binary_file", py::overload_cast<const Class&, const std::string&, size_t>(&writeEdgeListInBinaryFile<Label>))
    //.def("write_edgelist_in_binary_file", py::overload_cast<const Class&, std::ofstream&, size_t>(&writeEdgeListInBinaryFile<Label>))

    //.def("load_undirected_edgelist_from_binary_file", py::overload_cast<const std::string&, size_t>(&loadDirectedEdgeListFromBinaryFile<Label>))
    //.def("load_undirected_edgelist_from_binary_file", py::overload_cast<std::ifstream&, size_t>(&loadDirectedEdgeListFromBinaryFile<Label>))

    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label>))

    //.def("write_vertices_in_binary_file", py::overload_cast<const Class&, const std::string&, size_t>(&writeVerticesInBinaryFile<Label>))
    //.def("write_vertices_in_binary_file", py::overload_cast<const Class&, std::ofstream&, size_t>(&writeVerticesInBinaryFile<Label>))

    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, const std::string&, size_t>(&addVerticesFromBinaryFile<Label>))
    //.def("add_vertices_from_binary_file", py::overload_cast<Class&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label>));
}

#endif
