#include <pybind11/pybind11.h>
#include <acquisition/trajectories.h>
#include <acquisition/acquisitionsetup.h>
#include <mat/mat.h>

namespace py = pybind11;

void init_trajectories(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    namespace pr = protocols;

    auto p { m.def_submodule("protocols") };
    py::class_<pr::HelicalTrajectory, AbstractPreparationProtocol>(p, "HelicalTrajectory")
        .def(py::init<double, double, double, double>(),
            "angle_increment"_a, "pitch_increment"_a = 0.0, "start_pitch"_a = 0.0,
            "start_angle"_a = 0.0_deg)
        .def("is_applicable_to", &pr::HelicalTrajectory::isApplicableTo, "setup"_a)
        .def("set_angle_increment", &pr::HelicalTrajectory::setAngleIncrement, "angle_increment"_a)
        .def("set_pitch_increment", &pr::HelicalTrajectory::setPitchIncrement, "pitch_increment"_a)
        .def("set_start_pitch", &pr::HelicalTrajectory::setStartPitch, "start_pitch"_a)
        .def("set_start_angle", &pr::HelicalTrajectory::setStartAngle, "start_angle"_a);

    py::class_<pr::WobbleTrajectory, AbstractPreparationProtocol>(p, "WobbleTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "wobble_angle"_a = 15.0_deg, "wobble_freq"_a = 1.0)
        .def("is_applicable_to", &pr::WobbleTrajectory::isApplicableTo, "setup"_a)
        .def("set_start_angle", &pr::WobbleTrajectory::setStartAngle, "start_angle"_a)
        .def("set_wobble_angle", &pr::WobbleTrajectory::setWobbleAngle, "wobble_angle"_a)
        .def("set_wobble_freq", &pr::WobbleTrajectory::setWobbleFreq, "wobble_freq"_a);

    py::class_<pr::CirclePlusLineTrajectory, AbstractPreparationProtocol>(p,
        "CirclePlusLineTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "line_length"_a,
            "fraction_of_views_for_line"_a = 0.5, "start_angle"_a = 0.0_deg)
        .def("is_applicable_to", &pr::CirclePlusLineTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::ShortScanTrajectory, AbstractPreparationProtocol>(p,
        "ShortScanTrajectory")
        .def(py::init<double, double, double>(),
            "source_to_isocenter"_a, "start_angle"_a = 0.0_deg, "angle_span"_a = -1.0_deg)
        .def("is_applicable_to", &pr::ShortScanTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::AxialScanTrajectory, AbstractPreparationProtocol>(p,
        "AxialScanTrajectory")
        .def(py::init<double>(), "start_angle"_a = 0.0_deg)
        .def("is_applicable_to", &pr::AxialScanTrajectory::isApplicableTo, "setup"_a)
        .def("set_start_angle", &pr::AxialScanTrajectory::setStartAngle, "start_angle"_a);
}