#include "pybind11/pybind11.h"
#include "pybind11/stl.h"


namespace py = pybind11;


void defineAllGraphs(py::module &m);
void defineIOTools(py::module &m);


PYBIND11_MODULE(core, m){
    defineAllGraphs(m);
    defineIOTools(m);
}
