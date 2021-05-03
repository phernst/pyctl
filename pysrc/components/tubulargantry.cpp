#include "pysrc/pybind_common.h"
#include <components/tubulargantry.h>

namespace py = pybind11;

void init_tubulargantry(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<TubularGantry, AbstractGantry, std::shared_ptr<TubularGantry>>(m, "TubularGantry")
        .def(py::init([](double sourceToDetectorDistance,
                         double sourceToIsoCenterDistance,
                         double rotationAngle,
                         double pitchPosition,
                         double tiltAngle,
                         const std::string& name) -> TubularGantry
        {
            return {
                sourceToDetectorDistance,
                sourceToIsoCenterDistance,
                rotationAngle,
                pitchPosition,
                tiltAngle,
                QString::fromStdString(name)
            };
        }), "source_to_detector_distance"_a, "source_to_iso_center_distance"_a,
            "rotation_angle"_a = 0.0, "pitch_position"_a = 0.0, "tilt_angle"_a = 0.0,
            "name"_a = TubularGantry::defaultName().toStdString())
        .def("info", [](const TubularGantry& self) { return self.info().toStdString(); })
        .def("pitch_position", &TubularGantry::pitchPosition)
        .def("rotation_angle", &TubularGantry::rotationAngle)
        .def("tiltAngle", &TubularGantry::tiltAngle)
        .def("set_pitch_position", &TubularGantry::setPitchPosition, "position"_a)
        .def("set_rotation_angle", &TubularGantry::setRotationAngle, "angle"_a)
        .def("set_tilt_angle", &TubularGantry::setTiltAngle, "angle");
}
