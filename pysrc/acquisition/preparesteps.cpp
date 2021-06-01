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
        .def("is_applicable_to", &pr::TubularGantryParam::isApplicableTo, "system"_a)
        .def_static("for_rotation_angle", &pr::TubularGantryParam::forRotationAngle, "rotation"_a)
        .def_static("for_pitch_position", &pr::TubularGantryParam::forPitchPosition, "pitch"_a)
        .def_static("for_tilt_angle", &pr::TubularGantryParam::forTiltAngle, "tilt"_a);

    py::class_<pr::CarmGantryParam, AbstractPrepareStep>(m, "CarmGantryParam")
        .def(py::init<>())
        .def("set_location", &pr::CarmGantryParam::setLocation, "location"_a)
        .def("set_carm_span", &pr::CarmGantryParam::setCarmSpan, "span"_a)
        .def("prepare", &pr::CarmGantryParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::CarmGantryParam::isApplicableTo, "system"_a)
        .def_static("for_location", &pr::CarmGantryParam::forLocation, "location"_a)
        .def_static("for_carm_span", &pr::CarmGantryParam::forCarmSpan, "span"_a);

    py::class_<pr::GenericGantryParam, AbstractPrepareStep>(m, "GenericGantryParam")
        .def(py::init<>())
        .def("set_detector_location", &pr::GenericGantryParam::setDetectorLocation, "location"_a)
        .def("set_source_location", &pr::GenericGantryParam::setSourceLocation, "location"_a)
        .def("prepare", &pr::GenericGantryParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::GenericGantryParam::isApplicableTo, "system"_a)
        .def_static("for_detector_location", &pr::GenericGantryParam::forDetectorLocation, "location"_a)
        .def_static("for_source_location", &pr::GenericGantryParam::forSourceLocation, "location"_a);

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
        .def("is_applicable_to", &pr::GantryDisplacementParam::isApplicableTo, "system"_a)
        .def_static("for_detector_displacement", &pr::GantryDisplacementParam::forDetectorDisplacement,
            "displacement"_a)
        .def_static("for_gantry_displacement", &pr::GantryDisplacementParam::forGantryDisplacement,
            "displacement"_a)
        .def_static("for_source_displacement", &pr::GantryDisplacementParam::forSourceDisplacement,
            "displacement"_a)
        .def_static("for_detector_displacement_increment",
            &pr::GantryDisplacementParam::forDetectorDisplacementIncrement,
            "increment"_a)
        .def_static("for_source_displacement_increment",
            &pr::GantryDisplacementParam::forSourceDisplacementIncrement,
            "increment"_a);

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
        .def("is_applicable_to", &pr::SourceParam::isApplicableTo, "system"_a)
        .def_static("for_energy_range_restriction", &pr::SourceParam::forEnergyRangeRestriction,
            "range"_a)
        .def_static("for_flux_modifier", &pr::SourceParam::forFluxModifier,
            "modifier"_a)
        .def_static("for_focal_spot_size", [](py::tuple size)
        {
            if (size.size() != 2)
            {
                throw std::invalid_argument("size must be a tuple of 2 elements");
            }
            return pr::SourceParam::forFocalSpotSize({
                size[0].cast<double>(),
                size[1].cast<double>() });
        }, "size"_a)
        .def_static("for_focal_spot_position", &pr::SourceParam::forFocalSpotPosition,
            "position"_a);
    
    py::class_<pr::XrayLaserParam, pr::SourceParam>(m, "XrayLaserParam")
        .def(py::init<>())
        .def("set_photon_energy", &pr::XrayLaserParam::setPhotonEnergy, "energy"_a)
        .def("set_power", &pr::XrayLaserParam::setPower, "power"_a)
        .def("prepare", &pr::XrayLaserParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::XrayLaserParam::isApplicableTo, "system"_a)
        .def_static("for_photon_energy", &pr::XrayLaserParam::forPhotonEnergy,
            "energy"_a)
        .def_static("for_power", &pr::XrayLaserParam::forPower, "power"_a);

    py::class_<pr::XrayTubeParam, pr::SourceParam>(m, "XrayTubeParam")
        .def(py::init<>())
        .def("set_tube_voltage", &pr::XrayTubeParam::setTubeVoltage, "voltage"_a)
        .def("set_milliampere_seconds", &pr::XrayTubeParam::setMilliampereSeconds, "current"_a)
        .def("prepare", &pr::XrayTubeParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::XrayTubeParam::isApplicableTo, "system"_a)
        .def_static("for_tube_voltage", &pr::XrayTubeParam::forTubeVoltage,
            "voltage"_a)
        .def_static("for_milliampere_seconds", &pr::XrayTubeParam::forMilliampereSeconds,
            "mas"_a);

    py::class_<pr::GenericDetectorParam, AbstractPrepareStep>(m, "GenericDetectorParam")
        .def(py::init<>())
        .def("set_module_locations",
            [](pr::GenericDetectorParam& self, const std::vector<mat::Location>& locs)
        {
            self.setModuleLocations({ locs.cbegin(), locs.cend() });
        }, "module_locations"_a)
        .def("set_pixel_size", [](pr::GenericDetectorParam& self, py::tuple size)
        {
            if (size.size() != 2)
            {
                throw std::invalid_argument("size must be a tuple of 2 elements");
            }
            self.setPixelSize({ size[0].cast<double>(), size[1].cast<double>() });
        }, "size"_a)
        .def("set_skew_angle", &pr::GenericDetectorParam::setSkewAngle,
            "skew_angle"_a)
        .def("prepare", &pr::GenericDetectorParam::prepare, "system"_a)
        .def("is_applicable_to", &pr::GenericDetectorParam::isApplicableTo, "system"_a)
        .def_static("for_module_locations", [](const std::vector<mat::Location>& locs)
        {
            return pr::GenericDetectorParam::forModuleLocations(
                { locs.cbegin(), locs.cend() });
        }, "module_locations"_a)
        .def_static("for_pixel_size", [](py::tuple size)
        {
            if (size.size() != 2)
            {
                throw std::invalid_argument("size must be a tuple of 2 elements");
            }
            return pr::GenericDetectorParam::forPixelSize({
                size[0].cast<double>(),
                size[1].cast<double>() });
        }, "size"_a)
        .def_static("for_skew_angle", &pr::GenericDetectorParam::forSkewAngle,
            "skew_angle"_a);
}