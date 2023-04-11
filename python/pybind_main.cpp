#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

namespace py = pybind11;

void defineAllGraphs(py::module &m);
void defineIOTools(py::module &m);
void defineAlgorithms(py::module &m);

PYBIND11_MODULE(_core, m) {
    defineAllGraphs(m);
    py::module_ ioModule =
        m.def_submodule("io", "Read (write) graphs to (from) files.");
    defineIOTools(ioModule);
    py::module_ algorithmsModule =
        m.def_submodule("algorithms", "Common graph algorithms.");
    defineAlgorithms(algorithmsModule);
}
