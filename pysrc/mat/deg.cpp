#include "pysrc/pybind_common.h"
#include <mat/deg.h>

namespace py = pybind11;

void init_deg(py::module& m)
{
    using namespace py::literals;
    m.def("deg2rad", &operator"" _deg, "deg"_a);
}
