#ifndef BaseGraph_EDGE_LABELED_GRAPH_PYBIND_HPP
#define BaseGraph_EDGE_LABELED_GRAPH_PYBIND_HPP

#include <string>
#include <vector>

#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "BaseGraph/types.h"
#include "BaseGraph/edgelabeled_directedgraph.hpp"
#include "BaseGraph/edgelabeled_undirectedgraph.hpp"


namespace py = pybind11;
using namespace BaseGraph;


// Metafunctions that implement integer specific methods
template<typename EdgeLabel>
typename std::enable_if<!std::is_integral<EdgeLabel>::value>::type
    defineSpecializedEdgeLabeledDirectedGraph(py::class_<EdgeLabeledDirectedGraph<EdgeLabel>, DirectedGraph>& pyClass){}

template<typename EdgeLabel>
typename std::enable_if<std::is_integral<EdgeLabel>::value>::type
    defineSpecializedEdgeLabeledDirectedGraph(py::class_<EdgeLabeledDirectedGraph<EdgeLabel>, DirectedGraph>& pyClass){
        using Class = EdgeLabeledDirectedGraph<EdgeLabel>;

        pyClass
            .def("get_total_edge_number", [](const Class& self){ return self.getTotalEdgeNumber(); });
    }

template<typename EdgeLabel>
typename std::enable_if<!std::is_integral<EdgeLabel>::value>::type
    defineSpecializedEdgeLabeledUndirectedGraph(py::class_<EdgeLabeledUndirectedGraph<EdgeLabel>, UndirectedGraph>& pyClass){}

template<typename EdgeLabel>
typename std::enable_if<std::is_integral<EdgeLabel>::value>::type
    defineSpecializedEdgeLabeledUndirectedGraph(py::class_<EdgeLabeledUndirectedGraph<EdgeLabel>, UndirectedGraph>& pyClass){
        using Class = EdgeLabeledUndirectedGraph<EdgeLabel>;

        pyClass
            .def("get_total_edge_number", [](const Class& self){ return self.getTotalEdgeNumber(); });
    }


template<typename EdgeLabel>
void defineEdgeLabeledDirectedGraph(py::module& m, const std::string& typestr) {
    using Class = EdgeLabeledDirectedGraph<EdgeLabel>;
    std::string pyClassName = "EdgeLabeledDirectedGraph"+typestr;

    auto pyclass = py::class_<Class, DirectedGraph> (m, pyClassName.c_str());
    pyclass
        .def(py::init<size_t>(), py::arg("size"))
        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("label"), py::arg("force")=false)
        .def("add_reciprocal_edge_idx", py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addReciprocalEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"), py::arg("label"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&Class::removeEdgeIdx),
                                          py::arg("source index"), py::arg("destination index"))
        .def("has_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&Class::hasEdgeIdx, py::const_),
                                          py::arg("source index"), py::arg("destination index"))
        .def("remove_vertex_from_edgelist_idx", &Class::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_duplicate_edges",  &Class::removeDuplicateEdges)
        .def("remove_selfloops",        &Class::removeSelfLoops)
        .def("clear_edges",             &Class::clearEdges)

        .def("set_edge_label_to", py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::setEdgeLabelIdx),
                                    py::arg("source index"), py::arg("destination index"), py::arg("new label"), py::arg("force")=false)
        .def("get_edge_label_of", py::overload_cast<VertexIndex, VertexIndex, bool>(&Class::getEdgeLabelOfIdx, py::const_),
                                    py::arg("source index"), py::arg("destination index"), py::arg("exception if inexistent")=true)

        .def("get_deep_copy",        [](const Class& self) { return Class(self); })
        //.def("get_undirected_graph", [](const Class& self) { return EdgeLabeledUndirectedGraph<EdgeLabel>(self); })

        .def("__eq__",      [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
        .def("__str__",     [](const Class self)  { std::ostringstream ret; ret << self; return ret.str(); });

    defineSpecializedEdgeLabeledDirectedGraph<EdgeLabel>(pyclass);
}

template<typename EdgeLabel>
void defineEdgeLabeledUndirectedGraph(py::module& m, const std::string& typestr) {
    using Class = EdgeLabeledUndirectedGraph<EdgeLabel>;
    std::string pyClassName = "EdgeLabeledUndirectedGraph"+typestr;

    auto pyclass = py::class_<Class, UndirectedGraph> (m, pyClassName.c_str());
    pyclass
        .def(py::init<size_t>(), py::arg("size"))
        .def("add_edge_idx",            py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::addEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("label"), py::arg("force")=false)
        .def("remove_edge_idx",         py::overload_cast<VertexIndex, VertexIndex>(&Class::removeEdgeIdx),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("has_edge_idx",             py::overload_cast<VertexIndex, VertexIndex>(&Class::hasEdgeIdx, py::const_),
                                          py::arg("vertex1 index"), py::arg("vertex2 index"))
        .def("remove_vertex_from_edgelist_idx", &Class::removeVertexFromEdgeListIdx, py::arg("vertex index"))
        .def("remove_duplicate_edges",  &Class::removeDuplicateEdges)
        .def("remove_selfloops",        &Class::removeSelfLoops)
        .def("clear_edges",             &Class::clearEdges)

        .def("set_edge_label_to",    py::overload_cast<VertexIndex, VertexIndex, const EdgeLabel&, bool>(&Class::setEdgeLabelIdx),
                                       py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("new label"), py::arg("force")=false)
        .def("get_edge_label_of",    py::overload_cast<VertexIndex, VertexIndex, bool>(&Class::getEdgeLabelOfIdx, py::const_),
                                       py::arg("vertex1 index"), py::arg("vertex2 index"), py::arg("exception when inexistent")=true)

        .def("get_deep_copy",        [](const Class& self) { return Class(self); })
        //.def("get_directed_graph", [](const Class& self) { return self.getDirectedGraph(); })

        .def("__eq__",      [](const Class& self, const Class& other) {return self == other;}, py::is_operator())
        .def("__neq__",     [](const Class& self, const Class& other) {return self != other;}, py::is_operator())
        .def("__str__",     [](const Class self)  { std::ostringstream ret; ret << self; return ret.str(); });

    defineSpecializedEdgeLabeledUndirectedGraph<EdgeLabel>(pyclass);
}

template<typename EdgeLabel>
void defineEdgeLabeledGraphs(py::module& m, const std::string& typestr) {
    defineEdgeLabeledDirectedGraph<EdgeMultiplicity>(m, typestr);
    defineEdgeLabeledUndirectedGraph<EdgeMultiplicity>(m, typestr);
}


#endif
