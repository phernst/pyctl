#include <pybind11/pybind11.h>
#include <projectors/spectraleffectsextension.h>

namespace py = pybind11;

void init_spectraleffectsextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<SpectralEffectsExtension, ProjectorExtension>(m, "SpectralEffectsExtension")
        .def(py::init<>())
        .def(py::init<float>(), "energy_bin_width"_a)
        .def("project_composite", &SpectralEffectsExtension::projectComposite, "volume"_a)
        .def("is_linear", &SpectralEffectsExtension::isLinear)
        .def("set_spectral_sampling_resolution",
            &SpectralEffectsExtension::setSpectralSamplingResolution, "energy_bin_width"_a);
}
