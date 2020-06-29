#include <pybind11/pybind11.h>
#include <projectors/detectorsaturationextension.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_detectorsaturationextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<DetectorSaturationExtension, ProjectorExtension, PySharedPtr<DetectorSaturationExtension>>(m, "DetectorSaturationExtension")
        .def(py::init<>())
        .def(py::init<uint>(), "nb_spectral_samples"_a)
        .def("is_linear", &DetectorSaturationExtension::isLinear)
        .def("set_intensity_sampling", &DetectorSaturationExtension::setIntensitySampling,
            "nb_samples"_a)
        .def("configure", &DetectorSaturationExtension::configure, "setup"_a, py::keep_alive<1,2>());
}
