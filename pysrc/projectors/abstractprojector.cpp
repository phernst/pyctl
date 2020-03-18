#include <pybind11/pybind11.h>
#include <projectors/raycasterprojector.h>

namespace py = pybind11;

void init_abstractprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractProjector>(m, "AbstractProjector")
        .def("configure", &AbstractProjector::configure, "setup"_a, "config"_a)
        .def("project", &AbstractProjector::project, "volume"_a)
        .def("is_linear", &AbstractProjector::isLinear);
}
