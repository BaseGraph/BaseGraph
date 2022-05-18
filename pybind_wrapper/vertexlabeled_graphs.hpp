#ifndef BaseGraph_VERTEX_LABELED_GRAPH_PYBIND_HPP
#define BaseGraph_VERTEX_LABELED_GRAPH_PYBIND_HPP

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
void defineVertexLabeledUndirectedGraph(py::module &core, const std::string &classSuffix);

template<typename Label, bool isHashable>
void defineVertexLabeledDirectedGraph(py::module &core, const std::string &classSuffix);

template<typename Label, bool isHashable>
static typename std::enable_if<!std::is_same<Label, std::string>::value>::type
    defineBinaryIOVertexLabeledGraphs(py::module &io, const std::string& classSuffix);

template<typename Label, bool isHashable>
static typename std::enable_if<std::is_same<Label, std::string>::value>::type
    defineBinaryIOVertexLabeledGraphs(py::module &io, const std::string& classSuffix) {}


template<typename Label, bool isHashable>
void defineVertexLabeledGraphs(py::module &core, py::module &io, const std::string &classSuffix) {
    defineVertexLabeledDirectedGraph<Label, isHashable>(core, classSuffix);
    defineVertexLabeledUndirectedGraph<Label, isHashable>(core, classSuffix);
    defineBinaryIOVertexLabeledGraphs<Label, isHashable>(io, classSuffix);
}


template<typename Label, bool isHashable>
void defineVertexLabeledUndirectedGraph(py::module &core, const std::string &classSuffix) {
    using CppClass = VertexLabeledUndirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledUndirectedGraph") + classSuffix;

    auto pyClass = py::class_<CppClass, UndirectedGraph>(core, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

    pyClass
    .def(py::init<>())
    .def_static("from_base_class", [&](const UndirectedGraph& graph, std::vector<Label> vertices) { return CppClass(graph, vertices); })

    .def("is_vertex",                   &CppClass::isVertex, py::arg("vertex label"))
    .def("find_vertex_index",           [](const CppClass& self, Label v) { return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex",                  [](CppClass& self, Label v, bool force) { self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &CppClass::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("set_vertex_label_to",         [](CppClass& self, Label v1, Label v2) { self.setVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices",                &CppClass::getVertices)

    .def("add_edge",    &CppClass::addEdge, py::arg("vertex1 label"), py::arg("vertex2 label"), py::arg("force")=false)
    .def("is_edge",     &CppClass::isEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))
    .def("remove_edge", &CppClass::removeEdge, py::arg("vertex1 label"), py::arg("vertex2 label"))

    .def("write_test_edgelist", py::overload_cast<const CppClass&, const std::string&>(&writeTextEdgeList<Label, isHashable>),
                                          py::arg("file name"))

    .def("get_degree_of", [](const CppClass& self, Label v, bool withSelfLoops){ return self.getDegreeOfIdx(self.findVertexIndex(v), withSelfLoops); }, py::arg("vertex label"), py::arg("with self-loops")=true)

    .def("__eq__",  [](const CppClass& self, const CppClass& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const CppClass& self, const CppClass& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const CppClass& self) { std::ostringstream ret; ret << self; return ret.str(); });
}


template<typename Label, bool isHashable>
void defineVertexLabeledDirectedGraph(py::module &core, const std::string &classSuffix) {
    using CppClass = VertexLabeledDirectedGraph<Label, isHashable>;
    std::string pyclass_name = std::string("VertexLabeledDirectedGraph") + classSuffix;

    auto pyClass = py::class_<CppClass, DirectedGraph>(core, pyclass_name.c_str(), py::buffer_protocol(), py::dynamic_attr());

    pyClass
    .def(py::init<>())
    .def_static("from_base_class", [&](const DirectedGraph& graph, std::vector<Label> vertices) { return CppClass(graph, vertices); })

    .def("is_vertex",                   &CppClass::isVertex, py::arg("vertex label"))
    .def("find_vertex_index",           [](CppClass& self, Label v){ return self.findVertexIndex(v); }, py::arg("vertex label"))
    .def("add_vertex",                  [](CppClass& self, Label v, bool force){ self.addVertex(v, force); }, py::arg("vertex label"), py::arg("force")=false)
    .def("remove_vertex_from_edgelist", &CppClass::removeVertexFromEdgeList, py::arg("vertex label"))
    .def("set_vertex_label_to",         [](CppClass& self, Label v1, Label v2){ self.setVertexLabelTo(v1,v2); }, py::arg("previous label"), py::arg("new label"))
    .def("get_vertices",                &CppClass::getVertices)

    .def("add_edge",    &CppClass::addEdge, py::arg("source label"), py::arg("destination label"), py::arg("force")=false)
    .def("is_edge",     &CppClass::isEdge, py::arg("source label"), py::arg("destination label"))
    .def("remove_edge", &CppClass::removeEdge, py::arg("source label"), py::arg("destination label"))

    .def("get_in_degree_of",  [](const CppClass& self, Label v) { return self.getInDegreeOf(v); }, py::arg("vertex label"))
    .def("get_out_degree_of", [](const CppClass& self, Label v) { return self.getOutDegreeOf(v); }, py::arg("vertex label"))

    .def("write_test_edgelist", py::overload_cast<const CppClass&, const std::string&>(&writeTextEdgeList<Label, isHashable>),
                                          py::arg("file name"))

    .def("__eq__",  [](const CppClass& self, const CppClass& other) {return self == other;}, py::is_operator())
    .def("__neq__", [](const CppClass& self, const CppClass& other) {return self != other;}, py::is_operator())
    .def("__str__", [](const CppClass& self) { std::ostringstream ret; ret << self; return ret.str(); });
}

template<typename Label, bool isHashable>
static typename std::enable_if<!std::is_same<Label, std::string>::value>::type
defineIOVertexLabeledUndirectedGraph(py::module &io, const std::string& classSuffix) {
}

template<typename Label, bool isHashable>
static typename std::enable_if<!std::is_same<Label, std::string>::value>::type
defineBinaryIOVertexLabeledGraphs(py::module &io, const std::string& classSuffix) {
    using DirectedCppClass = VertexLabeledDirectedGraph<Label, isHashable>;
    using UndirectedCppClass = VertexLabeledUndirectedGraph<Label, isHashable>;

    io.def("write_binary_edgelist", [&](const DirectedCppClass& graph, const std::string& fileName) { writeBinaryEdgeList(graph, fileName); },
                                            py::arg("vertex labeled graph"), py::arg("file name"));
    io.def("write_binary_edgelist", [&](const UndirectedCppClass& graph, const std::string& fileName) { writeBinaryEdgeList(graph, fileName); },
                                            py::arg("vertex labeled graph"), py::arg("file name"));
    io.def("write_vertices_in_binary", [&](const DirectedCppClass& graph, const std::string& fileName) { writeVerticesToBinary(graph, fileName); },
                                            py::arg("vertex labeled graph"),py::arg("file name"));
    io.def("write_vertices_in_binary", [&](const UndirectedCppClass& graph, const std::string& fileName) { writeVerticesToBinary(graph, fileName); },
                                            py::arg("vertex labeled graph"),py::arg("file name"));

    io.def("add_vertices_from_binary", [&](DirectedCppClass& graph, const std::string& fileName) { addVerticesFromBinary(graph, fileName); },
                                            py::arg("vertex labeled graph"),py::arg("file name"));
    io.def("add_vertices_from_binary", [&](UndirectedCppClass& graph, const std::string& fileName) { addVerticesFromBinary(graph, fileName); },
                                            py::arg("vertex labeled graph"),py::arg("file name"));
    io.def(std::string("load_directed_binary_edgelist_"+classSuffix).c_str(),
           [](const std::string& fileName) { return loadDirectedBinaryEdgeList<Label, isHashable>(fileName); },
                                            py::arg("file name"));
    io.def(std::string("load_undirected_binary_edgelist_"+classSuffix).c_str(),
           [](const std::string& fileName) { return loadUndirectedBinaryEdgeList<Label, isHashable>(fileName); },
                                            py::arg("file name"));
    /*
    io.def("write_binary_edgelist",
           py::overload_cast<const UndirectedCppClass&, const std::string&, size_t>(&writeBinaryEdgeList<Label, isHashable>),
                   py::arg("vertex labeled graph"), py::arg("file name"));
    io.def("write_vertices_in_binary",
           py::overload_cast<const UndirectedCppClass&, const std::string&, size_t>(&writeVerticesToBinary<Label, isHashable>),
                   py::arg("vertex labeled graph"), py::arg("file name"));

    io.def("add_vertices_from_binary", py::overload_cast<UndirectedCppClass&,
           const std::string&, size_t>(&addVerticesFromBinary<Label, isHashable>), py::arg("file name"));
    io.def(std::string("load_undirected_binary_edgelist_"+classSuffix).c_str(),
           py::overload_cast<const std::string&, size_t>(&loadUndirectedBinaryEdgeList<Label, isHashable>),
                   py::arg("file name"));
    */
}

#endif
