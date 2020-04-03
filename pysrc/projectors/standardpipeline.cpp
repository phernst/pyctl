#include <pybind11/pybind11.h>
#include <projectors/standardpipeline.h>
#include <acquisition/acquisitionsetup.h>
#include <QSize>

namespace py = pybind11;

void init_standardpipeline(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto sp = py::class_<StandardPipeline, AbstractProjector>(m, "StandardPipeline");
    auto ap = py::enum_<StandardPipeline::ApproximationPolicy>(sp, "ApproximationPolicy");

    sp.def(py::init<StandardPipeline::ApproximationPolicy>(),
            "policy"_a = StandardPipeline::Default_Approximation)
        .def("configure", &StandardPipeline::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &StandardPipeline::project, "volume"_a)
        .def("project", [](StandardPipeline* self, const VoxelVolume<float>& vol)
        {
            return self->project(vol);
        }, "volume"_a)
        .def("project_composite", &StandardPipeline::projectComposite, "volume"_a)
        .def("is_linear", &StandardPipeline::isLinear)
        .def("enable_areal_focal_spot", &StandardPipeline::enableArealFocalSpot, "enable"_a = true)
        .def("enable_detector_saturation", &StandardPipeline::enableDetectorSaturation, "enable"_a = true)
        .def("enable_poisson_noise", &StandardPipeline::enablePoissonNoise, "enable"_a = true)
        .def("enable_spectral_effects", &StandardPipeline::enableSpectralEffects, "enable"_a = true);

    ap.value("No_Approximation", StandardPipeline::No_Approximation)
        .value("Default_Approximation", StandardPipeline::Default_Approximation)
        .value("Full_Approximation", StandardPipeline::Full_Approximation)
        .export_values();
}
