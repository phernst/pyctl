#include <pybind11/pybind11.h>
#include <projectors/abstractprojectorconfig.h>

namespace py = pybind11;

void init_abstractprojectorconfig(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractProjectorConfig>(m, "AbstractProjectorConfig")
        .def("clone", &AbstractProjectorConfig::clone);
}
