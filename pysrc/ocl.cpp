#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_clfileloader(py::module&);
void init_raycasterprojector(py::module&);

void init_ocl(py::module& m)
{
    init_clfileloader(m);
    init_raycasterprojector(m);
}