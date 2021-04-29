#include "pysrc/pybind_common.h"
#include <acquisition/preparesteps.h>

namespace py = pybind11;

void init_preparesteps(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    namespace pr = prepare;

    py::class_<pr::TubularGantryParam, AbstractPrepareStep>(m, "TubularGantryParam")
        .def(py::init<>())
        .def("set_rotation_angle", &pr::TubularGantryParam::setRotationAngle, "rotation"_a)
        .def("set_pitch_position", &pr::TubularGantryParam::setPitchPosition, "pitch"_a)
        .def("set_tilt_angle", &pr::TubularGantryParam::setTiltAngle, "tilt"_a)
        .def("prepare", &pr::TubularGantryParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::TubularGantryParam::isApplicableTo, "system"_a);

    py::class_<pr::CarmGantryParam, AbstractPrepareStep>(m, "CarmGantryParam")
        .def(py::init<>())
        .def("set_location", &pr::CarmGantryParam::setLocation, "location"_a)
        .def("set_carm_span", &pr::CarmGantryParam::setCarmSpan, "span"_a)
        .def("prepare", &pr::CarmGantryParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::CarmGantryParam::isApplicableTo, "system"_a);

    py::class_<pr::GenericGantryParam, AbstractPrepareStep>(m, "GenericGantryParam")
        .def(py::init<>())
        .def("set_detector_location", &pr::GenericGantryParam::setDetectorLocation, "location"_a)
        .def("set_source_location", &pr::GenericGantryParam::setSourceLocation, "location"_a)
        .def("prepare", &pr::GenericGantryParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::GenericGantryParam::isApplicableTo, "system"_a);

    py::class_<pr::GantryDisplacementParam, AbstractPrepareStep>(m, "GantryDisplacementParam")
        .def(py::init<>())
        .def("set_detector_placement", &pr::GantryDisplacementParam::setDetectorDisplacement,
            "displacement"_a)
        .def("set_gantry_displacement", &pr::GantryDisplacementParam::setGantryDisplacement,
            "displacement"_a)
        .def("set_source_displacement", &pr::GantryDisplacementParam::setSourceDisplacement,
            "displacement"_a)
        .def("increment_detector_displacement",
            &pr::GantryDisplacementParam::incrementDetectorDisplacement,
            "increment"_a)
        .def("increment_source_displacement",
            &pr::GantryDisplacementParam::incrementSourceDisplacement,
            "increment"_a)
        .def("prepare", &pr::GantryDisplacementParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::GantryDisplacementParam::isApplicableTo, "system"_a);

    py::class_<pr::SourceParam, AbstractPrepareStep>(m, "SourceParam")
        .def(py::init<>())
        .def("set_energy_range_restriction", &pr::SourceParam::setEnergyRangeRestriction,
            "range"_a)
        .def("set_flux_modifier", &pr::SourceParam::setFluxModifier, "modifier"_a)
        .def("set_focal_spot_size", [](pr::SourceParam& self, py::tuple size)
        {
            if (size.size() != 2)
            {
                throw std::invalid_argument("size must be a tuple of 2 elements");
            }
            self.setFocalSpotSize({ size[0].cast<double>(), size[1].cast<double>() });
        }, "size"_a)
        .def("set_focal_spot_position", &pr::SourceParam::setFocalSpotPosition, "position"_a)
        .def("prepare", &pr::SourceParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::SourceParam::isApplicableTo, "system"_a);
    
    py::class_<pr::XrayLaserParam, pr::SourceParam>(m, "XrayLaserParam")
        .def(py::init<>())
        .def("set_photon_energy", &pr::XrayLaserParam::setPhotonEnergy, "energy"_a)
        .def("set_power", &pr::XrayLaserParam::setPower, "power"_a)
        .def("prepare", &pr::XrayLaserParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::XrayLaserParam::isApplicableTo, "system"_a);

    py::class_<pr::XrayTubeParam, pr::SourceParam>(m, "XrayTubeParam")
        .def(py::init<>())
        .def("set_tube_voltage", &pr::XrayTubeParam::setTubeVoltage, "voltage"_a)
        .def("set_emission_current", &pr::XrayTubeParam::setEmissionCurrent, "current"_a)
        .def("prepare", &pr::XrayTubeParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::XrayTubeParam::isApplicableTo, "system"_a);
}