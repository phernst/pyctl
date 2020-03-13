#include <pysrc/ocl.h>
#include <pysrc/ocl/clfileloader.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_ocl(py::module& m)
{
    init_clfileloader(m);
}