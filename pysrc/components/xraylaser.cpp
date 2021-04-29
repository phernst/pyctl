#include "pysrc/pybind_common.h"
#include <components/xraylaser.h>

namespace py = pybind11;

static const py::tuple size_zero()
{
    auto result { py::tuple(2) };
    result[0] = 0.0;
    result[1] = 0.0;
    return result;
}

void init_xraylaser(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<XrayLaser, AbstractSource, std::shared_ptr<XrayLaser>>(m, "XrayLaser")
        .def("nominal_energy_range", &XrayLaser::nominalEnergyRange)
        .def(py::init([](py::tuple focalSpotSize,
                         const Vector3x1& focalSpotPosition,
                         double energy,
                         double output,
                         const std::string& name) -> XrayLaser
        {
            if (focalSpotSize.size() != 2)
            {
                throw std::invalid_argument("focal_spot_size must be a tuple of 2 elements");
            }
            return {
                { focalSpotSize[0].cast<double>(), focalSpotSize[1].cast<double>() },
                focalSpotPosition,
                energy,
                output,
                QString::fromStdString(name)
            };
        }), "focal_spot_size"_a = size_zero(), "focal_spot_position"_a = Vector3x1(0.0),
            "energy"_a = 100.0, "output"_a = 1.0, "name"_a = XrayLaser::defaultName().toStdString())
        .def("info", [](const XrayLaser& self) { return self.info().toStdString(); })
        .def("spectrum_discretization_hint", &XrayLaser::spectrumDiscretizationHint)
        .def("photon_energy", &XrayLaser::photonEnergy)
        .def("radiation_output", &XrayLaser::radiationOutput)
        .def("set_photon_energy", &XrayLaser::setPhotonEnergy, "energy"_a)
        .def("set_radiation_output", &XrayLaser::setRadiationOutput, "output"_a)
        .def_static("default_name", [](){ return XrayLaser::defaultName().toStdString(); });
}
