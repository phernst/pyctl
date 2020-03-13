#include <pysrc/ocl.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(_ctl, m) {
    init_ocl(m.def_submodule("ocl"));
}