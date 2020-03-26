#include <pybind11/pybind11.h>
#include <projectors/projectorextension.h>

namespace py = pybind11;

void init_projectorextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ProjectorExtension, AbstractProjector>(m, "ProjectorExtension")
        .def(py::init<AbstractProjector*>(), "projector"_a = nullptr)
        .def("project_composite", &ProjectorExtension::projectComposite, "volume"_a)
        .def("is_linear", &ProjectorExtension::isLinear)
        .def("use", static_cast<void(ProjectorExtension::*)(AbstractProjector*)>
            (&ProjectorExtension::use), "other"_a, py::keep_alive<1,2>())
        .def("release", &ProjectorExtension::release)
        .def("reset", &ProjectorExtension::reset);
}
