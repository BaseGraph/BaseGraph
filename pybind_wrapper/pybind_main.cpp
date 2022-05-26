#include "pybind11/pybind11.h"
#include "pybind11/stl.h"


namespace py = pybind11;


void defineAllGraphs(py::module &core, py::module &io);
void defineIOTools(py::module &m);
void defineRandomTools(py::module &m);
void defineMetrics(py::module &m);


PYBIND11_MODULE(basegraph, m){
    py::module core = m.def_submodule("core");
    py::module io = m.def_submodule("io");
    py::module random = m.def_submodule("random");
    py::module metrics = m.def_submodule("metrics");


    defineAllGraphs(core, io); // Templated graphs define their IO methods
    defineIOTools(io);
    defineRandomTools(random);
    defineMetrics(metrics);
}
