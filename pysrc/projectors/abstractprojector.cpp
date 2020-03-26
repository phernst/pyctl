#include <pybind11/pybind11.h>
#include <projectors/abstractprojector.h>
#include <acquisition/acquisitionsetup.h>

namespace py = pybind11;

void init_abstractprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractProjector>(m, "AbstractProjector")
        .def("configure", &AbstractProjector::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &AbstractProjector::project, "volume"_a)
        .def("project", [](AbstractProjector* p, const VoxelVolume<float>& volume)
        {
            return p->project(volume);
        })
        .def("is_linear", &AbstractProjector::isLinear)
        .def("project_composite", &AbstractProjector::projectComposite, "volume"_a);
}
