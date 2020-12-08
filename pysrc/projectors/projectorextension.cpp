#include <pybind11/pybind11.h>
#include <projectors/projectorextension.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_projectorextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ProjectorExtension, AbstractProjector, PySharedPtr<ProjectorExtension>>(m, "ProjectorExtension")
        .def(py::init([](PySharedPtr<AbstractProjector> proj)
        {
            return make_pysharedptr<ProjectorExtension>(proj.release_to_ctl());
        }), "projector"_a = nullptr)
        .def("project_composite", &ProjectorExtension::projectComposite, "volume"_a)
        .def("project_sparse", &ProjectorExtension::projectSparse, "volume"_a)
        .def("is_linear", &ProjectorExtension::isLinear)
        .def("use", [](ProjectorExtension& self, const PySharedPtr<AbstractProjector>& ptr)
        {
            self.use(ptr.release_to_ctl());
        }, "other"_a)
        .def("release", &ProjectorExtension::release)
        .def("reset", &ProjectorExtension::reset);
}
