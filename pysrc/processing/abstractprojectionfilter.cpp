#include <pybind11/pybind11.h>
#include <img/projectiondata.h>
#include <processing/abstractprojectionfilter.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_abstractprojectionfilter(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractProjectionFilter, PySharedPtr<AbstractProjectionFilter>>(m, "AbstractProjectionFilter")
        .def("filter", &AbstractProjectionFilter::filter, "projections"_a);
}
