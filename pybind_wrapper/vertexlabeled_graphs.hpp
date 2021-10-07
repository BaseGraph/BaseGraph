#ifndef BaseGraph_VERTEX_LABELED_GRAPH_PYBIND_H
#define BaseGraph_VERTEX_LABELED_GRAPH_PYBIND_H

#include <string>
#include <vector>
#include <sstream>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/vertexlabeled_graph.hpp"
#include "BaseGraph/fileio.h"


namespace py = pybind11;
using namespace BaseGraph;


template<typename Label, bool isHashable>
static void declareSpecializedVertexLabeledUndirectedGraph(py::class_<VertexLabeledUndirectedGraph<Label, isHashable>, UndirectedGraph>& pyClass);
template<bool isHashable>
static void declareSpecializedVertexLabeledUndirectedGraph(py::class_<VertexLabeledUndirectedGraph<std::string, isHashable>, UndirectedGraph>& pyClass) {}

template<typename Label, bool isHashable>
static void declareSpecializedVertexLabeledDirectedGraph(py::class_<VertexLabeledDirectedGraph<Label, isHashable>, DirectedGraph>& pyClass);
template<bool isHashable>
static void declareSpecializedVertexLabeledDirectedGraph(py::class_<VertexLabeledDirectedGraph<std::string, isHashable>, DirectedGraph>& pyClass) {}


template<typename Label, bool isHashable>
void declareVertexLabeledUndirectedGraph(py::module &m, const std::string &typestr) {
    using CppClass = VertexLabeledUndirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledUndirectedGraph") + typestr;

    auto pyClass = py::class_<CppClass, UndirectedGraph>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

    pyClass
    .def(py::init<>())
    .def_static("from_base_class", [&](const UndirectedGraph& graph, std::vector<Label> vertices) { return CppClass(graph, vertices); })

    .def("is_vertex",                   &CppClass::isVertex, py::arg("vertex label"))
    .def("find_vertex_index",           [](const CppClass& self, Label v) { return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex",                  [](CppClass& self, Label v, bool force) { self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &CppClass::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("set_vertex_label_to",      [](CppClass& self, Label v1, Label v2) { self.setVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices",                &CppClass::getVertices)

    .def("add_edge",    &CppClass::addEdge, py::arg("vertex1 label"), py::arg("vertex2 label"), py::arg("force")=false)
    .def("is_edge",     &CppClass::isEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))
    .def("remove_edge", &CppClass::removeEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))

    .def("write_edgelist_in_text_file",   py::overload_cast<const CppClass&, const std::string&>(&writeEdgeListInTextFile<Label, isHashable>))
    .def("write_edgelist_in_text_file",   py::overload_cast<const CppClass&, std::ofstream&>(&writeEdgeListInTextFile<Label, isHashable>))

    .def("get_degree_of",        [](const CppClass& self, Label v){ return self.getOutDegreeOf(v); }, py::arg("vertex label"))

    .def("__eq__",  [](const CppClass& self, const CppClass& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const CppClass& self, const CppClass& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const CppClass& self) { std::ostringstream ret; ret << self; return ret.str(); });

    declareSpecializedVertexLabeledUndirectedGraph(pyClass);
}


template<typename Label, bool isHashable>
void declareVertexLabeledDirectedGraph(py::module &m, const std::string &typestr) {
    using CppClass = VertexLabeledDirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledDirectedGraph") + typestr;

    auto pyClass = py::class_<CppClass, DirectedGraph>(m, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

    pyClass
    .def(py::init<>())
    .def_static("from_base_class", [](const DirectedGraph& graph, std::vector<Label> vertices) {return CppClass(graph, vertices);})

    .def("is_vertex",                   &CppClass::isVertex, py::arg("vertex label"))
    .def("find_vertex_index",           [](CppClass& self, Label v){ return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex",                  [](CppClass& self, Label v, bool force){ self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &CppClass::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("set_vertex_label_to",      [](CppClass& self, Label v1, Label v2){ self.setVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices",                &CppClass::getVertices)

    .def("add_edge",    &CppClass::addEdge, py::arg("source label"), py::arg("destination label"), py::arg("force")=false)
    .def("is_edge",     &CppClass::isEdge, py::arg("source label"), py::arg("destination label"))
    .def("remove_edge", &CppClass::removeEdge, py::arg("source label"), py::arg("destination label"))

    .def("get_in_degree_of",  [](const CppClass& self, Label v) { return self.getInDegreeOf(v); }, py::arg("vertex label"))
    .def("get_out_degree_of", [](const CppClass& self, Label v) { return self.getOutDegreeOf(v); }, py::arg("vertex label"))

    .def("write_edgelist_in_text_file",   py::overload_cast<const CppClass&, const std::string&>(&writeEdgeListInTextFile<Label, isHashable>))
    .def("write_edgelist_in_text_file",   py::overload_cast<const CppClass&, std::ofstream&>(&writeEdgeListInTextFile<Label, isHashable>))

    .def("__eq__",  [](const CppClass& self, const CppClass& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const CppClass& self, const CppClass& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const CppClass& self) { std::ostringstream ret; ret << self; return ret.str(); });

    declareSpecializedVertexLabeledDirectedGraph(pyClass);
}


template<typename Label, bool isHashable>
static void declareSpecializedVertexLabeledUndirectedGraph(py::class_<VertexLabeledUndirectedGraph<Label, isHashable>, UndirectedGraph>& pyClass) {
    using CppClass = VertexLabeledUndirectedGraph<Label, isHashable>;

    pyClass
    .def("write_edgelist_in_binary_file", py::overload_cast<const CppClass&, const std::string&, size_t>(&writeEdgeListInBinaryFile<Label, isHashable>))
    .def("write_edgelist_in_binary_file", py::overload_cast<const CppClass&, std::ofstream&, size_t>(&writeEdgeListInBinaryFile<Label, isHashable>))
    .def("write_vertices_in_binary_file", py::overload_cast<const CppClass&, const std::string&, size_t>(&writeVerticesInBinaryFile<Label, isHashable>))
    .def("write_vertices_in_binary_file", py::overload_cast<const CppClass&, std::ofstream&, size_t>(&writeVerticesInBinaryFile<Label, isHashable>))

    .def("load_undirected_edgelist_from_binary_file", py::overload_cast<const std::string&, size_t>(&loadUndirectedEdgeListFromBinaryFile<Label, isHashable>))
    .def("load_undirected_edgelist_from_binary_file", py::overload_cast<std::ifstream&, size_t>(&loadUndirectedEdgeListFromBinaryFile<Label, isHashable>))
    .def("add_vertices_from_binary_file", py::overload_cast<CppClass&, const std::string&, size_t>(&addVerticesFromBinaryFile<Label, isHashable>))
    .def("add_vertices_from_binary_file", py::overload_cast<CppClass&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label, isHashable>));
}

template<typename Label, bool isHashable>
static void declareSpecializedVertexLabeledDirectedGraph(py::class_<VertexLabeledDirectedGraph<Label, isHashable>, DirectedGraph>& pyClass) {
    using CppClass = VertexLabeledUndirectedGraph<Label, isHashable>;

    pyClass
    .def("write_edgelist_in_binary_file", py::overload_cast<const CppClass&, const std::string&, size_t>(&writeEdgeListInBinaryFile<Label, isHashable>))
    .def("write_edgelist_in_binary_file", py::overload_cast<const CppClass&, std::ofstream&, size_t>(&writeEdgeListInBinaryFile<Label, isHashable>))
    .def("write_vertices_in_binary_file", py::overload_cast<const CppClass&, const std::string&, size_t>(&writeVerticesInBinaryFile<Label, isHashable>))
    .def("write_vertices_in_binary_file", py::overload_cast<const CppClass&, std::ofstream&, size_t>(&writeVerticesInBinaryFile<Label, isHashable>))

    .def("load_undirected_edgelist_from_binary_file", py::overload_cast<const std::string&, size_t>(&loadDirectedEdgeListFromBinaryFile<Label, isHashable>))
    .def("load_undirected_edgelist_from_binary_file", py::overload_cast<std::ifstream&, size_t>(&loadDirectedEdgeListFromBinaryFile<Label, isHashable>))
    .def("add_vertices_from_binary_file", py::overload_cast<CppClass&, const std::string&, size_t>(&addVerticesFromBinaryFile<Label, isHashable>))
    .def("add_vertices_from_binary_file", py::overload_cast<CppClass&, std::ifstream&, size_t>(&addVerticesFromBinaryFile<Label, isHashable>));
}

#endif
