#include <pybind11/pybind11.h>
#include <acquisition/trajectories.h>
#include "mat/mat.h"

namespace py = pybind11;

void init_trajectories(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto p { m.def_submodule("protocols") };
    py::class_<protocols::HelicalTrajectory, AbstractPreparationProtocol>(p, "HelicalTrajectory")
        .def(py::init<double, double, double, double>(),
            "angle_increment"_a, "pitch_increment"_a = 0.0, "start_pitch"_a = 0.0,
            "start_angle"_a = 0.0_deg);

    py::class_<protocols::WobbleTrajectory, AbstractPreparationProtocol>(p, "WobbleTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "wobble_angle"_a = 15.0_deg, "wobble_freq"_a = 1.0);

    py::class_<protocols::CirclePlusLineTrajectory, AbstractPreparationProtocol>(p,
        "CirclePlusLineTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "line_length"_a,
            "fraction_of_views_for_line"_a = 0.5, "start_angle"_a = 0.0_deg);

    py::class_<protocols::ShortScanTrajectory, AbstractPreparationProtocol>(p,
        "ShortScanTrajectory")
        .def(py::init<double, double, double>(),
            "source_to_isocenter"_a, "start_angle"_a = 0.0_deg, "angle_span"_a = -1.0_deg);

    py::class_<protocols::AxialScanTrajectory, AbstractPreparationProtocol>(p,
        "AxialScanTrajectory")
        .def(py::init<double>(),
            "start_angle"_a = 0.0_deg);
}