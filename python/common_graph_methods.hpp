#ifndef BaseGraph_COMMON_GRAPH_METHODS_HPP
#define BaseGraph_COMMON_GRAPH_METHODS_HPP

#include <sstream>
#include <string>
#include <vector>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/directed_graph.hpp"
#include "BaseGraph/types.h"
#include "BaseGraph/undirected_graph.hpp"

namespace py = pybind11;
using namespace BaseGraph;

template <template <class...> class Graph, typename EdgeLabel>
void defineCommonLabeledGraphMethods(py::class_<Graph<EdgeLabel>> &pyClass) {
    using Class = Graph<EdgeLabel>;
    pyClass
        .def("add_edge",
             py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel &,
                               bool>(&Class::addEdge),
             py::arg("source index"), py::arg("destination index"),
             py::arg("label"), py::arg("force") = false)
        .def("has_edge",
             py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel &>(
                 &Class::hasEdge, py::const_),
             py::arg("source index"), py::arg("destination index"),
             py::arg("label"))
        .def("set_edge_label",
             py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel &,
                               bool>(&Class::setEdgeLabel),
             py::arg("source index"), py::arg("destination index"),
             py::arg("new label"), py::arg("force") = false)
        .def("get_edge_label",
             py::overload_cast<VertexIndex, VertexIndex, bool>(
                 &Class::getEdgeLabel, py::const_),
             py::arg("source index"), py::arg("destination index"),
             py::arg("exception if inexistent") = true);
}

template <typename Graph>
void defineCommonGraphMethods(py::class_<Graph> &pyClass) {
    pyClass
        .def(
            "resize", [](Graph &self, size_t newSize) { self.resize(newSize); },
            py::arg("size"))
        .def("get_size", [&](const Graph &self) { return self.getSize(); })
        .def("get_edge_number",
             [](const Graph &self) { return self.getEdgeNumber(); })
        .def("add_edge",
             py::overload_cast<VertexIndex, VertexIndex, bool>(&Graph::addEdge),
             py::arg("source index"), py::arg("destination index"),
             py::arg("force") = false)
        .def("remove_edge",
             py::overload_cast<VertexIndex, VertexIndex>(&Graph::removeEdge),
             py::arg("source index"), py::arg("destination index"))
        .def("has_edge",
             py::overload_cast<VertexIndex, VertexIndex>(&Graph::hasEdge,
                                                         py::const_),
             py::arg("source index"), py::arg("destination index"))
        .def(
            "remove_vertex_from_edgelist",
            [](Graph &self, VertexIndex vertex) {
                self.removeVertexFromEdgeList(vertex);
            },
            py::arg("vertex index"))
        .def("remove_duplicate_edges",
             [](Graph &self) { self.removeDuplicateEdges(); })
        .def("remove_selfloops", [](Graph &self) { self.removeSelfLoops(); })
        .def("clear_edges", [](Graph &self) { self.clearEdges(); })
        .def(
            "get_out_neighbours",
            [](const Graph &self, VertexIndex vertex) {
                return self.getOutNeighbours(vertex);
            },
            py::arg("vertex index"))
        .def("get_deep_copy", [](const Graph &self) { return Graph(self); })
        .def("get_adjacency_matrix", &Graph::getAdjacencyMatrix)

        .def("edges", [](const Graph &self) { return self.edges(); })

        .def(
            "__eq__",
            [](const Graph &self, const Graph &other) { return self == other; },
            py::is_operator())
        .def(
            "__neq__",
            [](const Graph &self, const Graph &other) { return self != other; },
            py::is_operator())
        .def("__getitem__",
             [](const Graph &self, VertexIndex idx) {
                 return self.getOutNeighbours(idx);
             })
        .def("__str__",
             [](const Graph &self) {
                 std::ostringstream ret;
                 ret << self;
                 return ret.str();
             })
        .def(
            "__iter__",
            [](const Graph &self) {
                return py::make_iterator(self.begin(), self.end());
            }, // Essential: keep object alive while iterator exists
            py::keep_alive<0, 1>())
        .def("__len__", [](const Graph &self) { return self.getSize(); });
}

template <typename EdgeLabel>
void defineLabeledDirectedGraph(py::module &m, const std::string &typestr) {
    using Class = LabeledDirectedGraph<EdgeLabel>;
    std::string pyClassName = "LabeledDirectedGraph" + typestr;
    if (typestr == "")
        pyClassName = "DirectedGraph";

    auto pyClass = py::class_<Class>(m, pyClassName.c_str());
    pyClass.def(py::init<size_t>(), py::arg("size"))
        .def("is_directed", [](const Class &self) { return true; })
        .def("add_reciprocal_edge",
             py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel &,
                               bool>(&Class::addReciprocalEdge),
             py::arg("source index"), py::arg("destination index"),
             py::arg("label"), py::arg("force") = false)
        .def("get_in_degree", &Class::getInDegree, py::arg("vertex index"))
        .def("get_in_degrees", &Class::getInDegrees)
        .def("get_out_degree", &Class::getOutDegree, py::arg("vertex index"))
        .def("get_out_degrees", &Class::getOutDegrees)
        .def("get_reversed_graph", &Class::getReversedGraph)
        .def("to_undirected_graph", [](const Class &self) {
            return LabeledUndirectedGraph<EdgeLabel>(self);
        });

    defineCommonGraphMethods(pyClass);
    defineCommonLabeledGraphMethods(pyClass);

    py::class_<typename Class::Edges>(
        m, std::string("DirectedEdgeIterator" + typestr).c_str())
        .def("__iter__", [](const typename Class::Edges &self) {
            return py::make_iterator(self.begin(), self.end());
        });
}

template <typename EdgeLabel>
void defineLabeledUndirectedGraph(py::module &m, const std::string &typestr) {
    using Class = LabeledUndirectedGraph<EdgeLabel>;
    std::string pyClassName = "LabeledUndirectedGraph" + typestr;
    if (typestr == "")
        pyClassName = "UndirectedGraph";

    auto pyClass = py::class_<Class>(m, pyClassName.c_str());
    pyClass.def(py::init<size_t>(), py::arg("size"))
        .def("is_directed", [](const Class &self) { return false; })
        .def("get_neighbours", &Class::getNeighbours, py::arg("vertex index"))
        .def("get_degree", &Class::getDegree, py::arg("vertex index"),
             py::arg("count self-loops twice") = true)
        .def("get_degrees", &Class::getDegrees,
             py::arg("count self-loops twice") = true)
        .def("to_directed_graph",
             [](const Class &self) { return self.getDirectedGraph(); });

    defineCommonGraphMethods(pyClass);
    defineCommonLabeledGraphMethods(pyClass);

    py::class_<typename Class::Edges>(
        m, std::string("UndirectedEdgeIterator" + typestr).c_str())
        .def("__iter__", [](const typename Class::Edges &self) {
            return py::make_iterator(self.begin(), self.end());
        });
}

#endif
