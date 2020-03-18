#include <pybind11/pybind11.h>
#include <projectors/raycasterprojector.h>
#include <projectors/abstractprojectorconfig.h>
#include <components/abstractdetector.h>

namespace py = pybind11;

void init_raycasterprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto rcp = py::class_<OCL::RayCasterProjector, AbstractProjector>(m, "RayCasterProjector")
        .def(py::init<>())
        .def("configure", &OCL::RayCasterProjector::configure, "setup"_a, "config"_a)
        .def("project", &OCL::RayCasterProjector::project, "volume"_a);

    py::class_<OCL::RayCasterProjector::Config, AbstractProjectorConfig>(rcp, "Config")
        .def(py::init<>())
        .def("clone", &OCL::RayCasterProjector::Config::clone)
        .def_static("optimized_for", &OCL::RayCasterProjector::Config::optimizedFor,
            "volume"_a, "detector"_a);
}
