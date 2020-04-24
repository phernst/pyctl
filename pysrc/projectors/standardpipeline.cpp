#include <pybind11/pybind11.h>
#include <projectors/standardpipeline.h>
#include <acquisition/acquisitionsetup.h>
#include <QSize>

namespace py = pybind11;

// TODO - find more generic way of binding the private settings classes

class PySAFS
{
public:
    PySAFS(CTL::StandardPipeline& sp): _sp{sp} {}
    void setDiscretization(const QSize& discretization)
    {
        _sp.settingsArealFocalSpot().setDiscretization(discretization);
    }
    void enableLowExtinctionApproximation(bool enable = true)
    {
        _sp.settingsArealFocalSpot().enableLowExtinctionApproximation(enable);
    }

private:
    CTL::StandardPipeline& _sp;
};

class PySDS
{
public:
    PySDS(CTL::StandardPipeline& sp): _sp{sp} {}
    void setSpectralSamples(uint nbSamples)
    {
        _sp.settingsDetectorSaturation().setSpectralSamples(nbSamples);
    }

private:
    CTL::StandardPipeline& _sp;
};

class PyPN
{
public:
    PyPN(CTL::StandardPipeline& sp): _sp{sp} {}
    void setFixedSeed(uint seed)
    {
        _sp.settingsPoissonNoise().setFixedSeed(seed);
    }
    void setRandomSeedMode()
    {
        _sp.settingsPoissonNoise().setRandomSeedMode();
    }
    void setParallelizationMode(bool enabled)
    {
        _sp.settingsPoissonNoise().setParallelizationMode(enabled);
    }

private:
    CTL::StandardPipeline& _sp;
};

class PySE
{
public:
    PySE(CTL::StandardPipeline& sp): _sp{sp} {}
    void setSamplingResolution(float energyBinWidth)
    {
        _sp.settingsSpectralEffects().setSamplingResolution(energyBinWidth);
    }

private:
    CTL::StandardPipeline& _sp;
};

class PyRC
{
public:
    PyRC(CTL::StandardPipeline& sp): _sp{sp} {}
    void setInterpolation(bool enabled)
    {
        _sp.settingsRayCaster().setInterpolation(enabled);
    }
    void setRaysPerPixel(const QSize& sampling)
    {
        _sp.settingsRayCaster().setRaysPerPixel(sampling);
    }
    void setRaySampling(float sampling)
    {
        _sp.settingsRayCaster().setRaySampling(sampling);
    }
    // TODO - change argument name in v0.3.2
    void setVolumeUpSampling(uint upsamplingFactor)
    {
        _sp.settingsRayCaster().setVolumeUpSampling(upsamplingFactor);
    }

private:
    CTL::StandardPipeline& _sp;
};

void init_standardpipeline(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto sp = py::class_<StandardPipeline, AbstractProjector>(m, "StandardPipeline");
    auto afs = py::class_<PySAFS>(sp, "SettingsAFS");
    auto ds = py::class_<PySDS>(sp, "SettingsDetectorSaturation");
    auto pn = py::class_<PyPN>(sp, "SettingsPoissonNoise");
    auto se = py::class_<PySE>(sp, "SettingsSpectralEffects");
    auto rc = py::class_<PyRC>(sp, "SettingsRayCaster");
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
        .def("enable_spectral_effects", &StandardPipeline::enableSpectralEffects, "enable"_a = true)
        .def("settings_areal_focal_spot", [](StandardPipeline& self) -> PySAFS { return self; })
        .def("settings_detector_saturation", [](StandardPipeline& self) -> PySDS { return self; })
        .def("settings_poisson_noise", [](StandardPipeline& self) -> PyPN { return self; })
        .def("settings_spectral_effects", [](StandardPipeline& self) -> PySE { return self; })
        .def("settings_ray_caster", [](StandardPipeline& self) -> PyRC { return self; });

    afs.def("set_discretization", [](PySAFS& self, py::tuple disc)
        {
            self.setDiscretization({ disc[0].cast<int>(), disc[1].cast<int>() });
        }, "discretization"_a)
        .def("enable_low_extinction_approximation", &PySAFS::enableLowExtinctionApproximation,
            "enable"_a = true);

    ds.def("set_spectral_samples", &PySDS::setSpectralSamples, "nb_samples"_a);

    pn.def("set_fixed_seed", &PyPN::setFixedSeed, "seed"_a)
        .def("set_random_seed_mode", &PyPN::setRandomSeedMode)
        .def("set_parallelization_mode", &PyPN::setParallelizationMode, "enabled"_a = true);

    se.def("set_sampling_resolution", &PySE::setSamplingResolution, "energy_bin_width"_a);

    rc.def("set_interpolation", &PyRC::setInterpolation, "enabled"_a)
        .def("set_rays_per_pixel", [](PyRC& self, py::tuple disc)
        {
            self.setRaysPerPixel({ disc[0].cast<int>(), disc[1].cast<int>() });
        }, "sampling"_a)
        .def("set_ray_sampling", &PyRC::setRaySampling, "sampling"_a)
        // TODO - change argument name in v0.3.2
        .def("set_volume_up_sampling", &PyRC::setVolumeUpSampling, "upsampling_factor"_a);

    ap.value("No_Approximation", StandardPipeline::No_Approximation)
        .value("Default_Approximation", StandardPipeline::Default_Approximation)
        .value("Full_Approximation", StandardPipeline::Full_Approximation)
        .export_values();
}
