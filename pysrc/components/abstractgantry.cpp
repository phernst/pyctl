#include <pybind11/pybind11.h>
#include <components/abstractgantry.h>

namespace py = pybind11;

void init_abstractgantry(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<AbstractGantry, SystemComponent, std::shared_ptr<AbstractGantry>>(m, "AbstractGantry")
        .def("info", [](AbstractGantry& self) { return self.info().toStdString(); })
        .def("source_location", &AbstractGantry::sourceLocation)
        .def("detector_location", &AbstractGantry::detectorLocation)
        .def("detector_displacement", &AbstractGantry::detectorDisplacement)
        .def("gantry_displacement", &AbstractGantry::gantryDisplacement)
        .def("source_displacement", &AbstractGantry::sourceDisplacement)
        .def("set_detector_displacement", &AbstractGantry::setDetectorDisplacement,
            "displacement"_a)
        .def("set_gantry_displacement", &AbstractGantry::setGantryDisplacement,
            "displacement"_a)
        .def("set_source_displacement", &AbstractGantry::setSourceDisplacement,
            "displacement"_a)
        .def("source_position", &AbstractGantry::sourcePosition)
        .def("source_rotation", &AbstractGantry::sourceRotation)
        .def("detector_position", &AbstractGantry::detectorPosition)
        .def("detector_rotation", &AbstractGantry::detectorRotation)
        .def("set_detector_displacement_angles", &AbstractGantry::setDetectorDisplacementAngles,
            "roll_angle"_a, "tilt_angle"_a, "twist_angle"_a)
        .def("set_detector_displacement_position", static_cast<void(AbstractGantry::*)
            (const Vector3x1&)>(&AbstractGantry::setDetectorDisplacementPosition),
            "position"_a)
        .def("set_detector_displacement_position", static_cast<void(AbstractGantry::*)
            (double,double,double)>(&AbstractGantry::setDetectorDisplacementPosition),
            "x"_a, "y"_a, "z"_a)
        .def("set_gantry_displacement_position", static_cast<void(AbstractGantry::*)
            (const Vector3x1&)>(&AbstractGantry::setGantryDisplacementPosition),
            "position"_a)
        .def("set_gantry_displacement_position", static_cast<void(AbstractGantry::*)
            (double,double,double)>(&AbstractGantry::setGantryDisplacementPosition),
            "x"_a, "y"_a, "z"_a)
        .def("set_source_displacement_position", static_cast<void(AbstractGantry::*)
            (const Vector3x1&)>(&AbstractGantry::setSourceDisplacementPosition),
            "position"_a)
        .def("set_source_displacement_position", static_cast<void(AbstractGantry::*)
            (double,double,double)>(&AbstractGantry::setSourceDisplacementPosition),
            "x"_a, "y"_a, "z"_a);
}
