#include "pysrc/pybind_common.h"
#include <img/compositevolume.h>
#include <img/sparsevoxelvolume.h>
#include <projectors/dynamicprojectorextension.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_dynamicprojectorextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<DynamicProjectorExtension, ProjectorExtension, PySharedPtr<DynamicProjectorExtension>>(m, "DynamicProjectorExtension")
        .def(py::init<>())
        .def("configure", &DynamicProjectorExtension::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &DynamicProjectorExtension::project, "volume"_a)
        .def("project_composite", &DynamicProjectorExtension::projectComposite, "volume"_a)
        .def("project_sparse", &DynamicProjectorExtension::projectSparse, "volume"_a);
}
