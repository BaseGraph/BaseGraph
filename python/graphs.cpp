#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "BaseGraph/directed_multigraph.hpp"
#include "BaseGraph/undirected_multigraph.hpp"

#include "common_graph_methods.hpp"

namespace py = pybind11;
using namespace BaseGraph;

static void defineGraphs(py::module &m);
static void defineAllLabeledGraphs(py::module &m);
static void defineMultigraphs(py::module &m);
template <typename EdgeLabel>
void defineLabeledGraphs(py::module &m, const std::string &typestr);

void defineAllGraphs(py::module &m) {
    defineGraphs(m);
    defineMultigraphs(m);
    defineAllLabeledGraphs(m);
}

void defineGraphs(py::module &m) {
    defineLabeledDirectedGraph<BaseGraph::NoLabel>(m, "");
    defineLabeledUndirectedGraph<BaseGraph::NoLabel>(m, "");
}

static void defineAllLabeledGraphs(py::module &m) {
    defineLabeledGraphs<EdgeMultiplicity>(m, "UnsignedInt");
}

static void defineMultigraphs(py::module &m) {
    auto pyDirectedMultigraph =
        py::class_<DirectedMultigraph>(m, "DirectedMultigraph");
    pyDirectedMultigraph.def(py::init<size_t>(), py::arg("size"))
        .def("get_total_edge_number", &DirectedMultigraph::getTotalEdgeNumber)
        .def(
            "add_multiedge",
            py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(
                &DirectedMultigraph::addMultiedge),
            py::arg("source"), py::arg("destination"), py::arg("multiplicity"),
            py::arg("force") = false)
        .def("remove_multiedge",
             py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(
                 &DirectedMultigraph::removeMultiedge),
             py::arg("source"), py::arg("destination"), py::arg("multiplicity"))
        .def("get_edge_multiplicity",
             py::overload_cast<VertexIndex, VertexIndex>(
                 &DirectedMultigraph::getEdgeMultiplicity, py::const_),
             py::arg("source"), py::arg("destination"))
        .def("set_edge_multiplicity",
             py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(
                 &DirectedMultigraph::setEdgeMultiplicity),
             py::arg("source"), py::arg("destination"),
             py::arg("multiplicity"));

    defineCommonGraphMethods(pyDirectedMultigraph);

    auto pyUndirectedMultigraph =
        py::class_<UndirectedMultigraph>(m, "UndirectedMultigraph");
    pyUndirectedMultigraph.def(py::init<size_t>(), py::arg("size"))
        .def("get_total_edge_number", &UndirectedMultigraph::getTotalEdgeNumber)
        .def(
            "add_multiedge",
            py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity, bool>(
                &UndirectedMultigraph::addMultiedge),
            py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"),
            py::arg("force") = false)
        .def("remove_multiedge",
             py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(
                 &UndirectedMultigraph::removeMultiedge),
             py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"))
        .def("get_edge_multiplicity",
             py::overload_cast<VertexIndex, VertexIndex>(
                 &UndirectedMultigraph::getEdgeMultiplicity, py::const_),
             py::arg("vertex1"), py::arg("vertex2"))
        .def("set_edge_multiplicity",
             py::overload_cast<VertexIndex, VertexIndex, EdgeMultiplicity>(
                 &UndirectedMultigraph::setEdgeMultiplicity),
             py::arg("vertex1"), py::arg("vertex2"), py::arg("multiplicity"));

    defineCommonGraphMethods(pyUndirectedMultigraph);
}

template <typename EdgeLabel>
void defineLabeledGraphs(py::module &m, const std::string &typestr) {
    defineLabeledDirectedGraph<EdgeLabel>(m, typestr);
    defineLabeledUndirectedGraph<EdgeLabel>(m, typestr);
}
