#include <pybind11/pybind11.h>
#include <projectors/abstractprojector.h>
#include <acquisition/acquisitionsetup.h>
#include "pysrc/pysharedptr.h"

namespace py = pybind11;

void init_abstractprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractProjector, PySharedPtr<AbstractProjector>>(m, "AbstractProjector")
        .def("configure", &AbstractProjector::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &AbstractProjector::project, "volume"_a)
        .def("project", [](AbstractProjector& self, const VoxelVolume<float>& volume)
        {
            return self.project(volume);
        })
        .def("is_linear", &AbstractProjector::isLinear)
        .def("project_composite", &AbstractProjector::projectComposite, "volume"_a)
        .def("project_sparse", &AbstractProjector::projectSparse, "volume"_a)
        .def("configure_and_project", [](AbstractProjector& self,
                                         const AcquisitionSetup& setup,
                                         const VolumeData& volume)
        {
            return self.configureAndProject(setup, volume);
        }, "setup"_a, "volume"_a)
        .def("configure_and_project", [](AbstractProjector& self,
                                         const AcquisitionSetup& setup,
                                         const CompositeVolume& volume)
        {
            return self.configureAndProject(setup, volume);
        }, "setup"_a, "volume"_a);
}
