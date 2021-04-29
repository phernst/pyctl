#include "pysrc/pybind_common.h"
#include <acquisition/trajectories.h>
#include <acquisition/acquisitionsetup.h>
#include <mat/mat.h>

namespace py = pybind11;

void init_trajectories(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    namespace pr = protocols;

    py::class_<pr::HelicalTrajectory, AbstractPreparationProtocol>(m, "HelicalTrajectory")
        .def(py::init<double, double, double, double>(),
            "angle_increment"_a, "pitch_increment"_a = 0.0, "start_pitch"_a = 0.0,
            "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::HelicalTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::HelicalTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("angle_increment", &pr::HelicalTrajectory::angleIncrement)
        .def_readwrite("pitch_increment", &pr::HelicalTrajectory::pitchIncrement)
        .def_readwrite("start_pitch", &pr::HelicalTrajectory::startPitch)
        .def_readwrite("start_angle", &pr::HelicalTrajectory::startAngle);

    py::class_<pr::WobbleTrajectory, AbstractPreparationProtocol>(m, "WobbleTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "source_to_isocenter"_a, "start_angle"_a = 0.0_deg, "angle_span"_a = -1.0_deg,
            "wobble_angle"_a = 15.0_deg, "wobble_freq"_a = 1.0)
        .def("prepare_steps", &pr::WobbleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::WobbleTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("source_to_isocenter", &pr::WobbleTrajectory::sourceToIsocenter)
        .def_readwrite("start_angle", &pr::WobbleTrajectory::startAngle)
        .def_readwrite("angle_span", &pr::WobbleTrajectory::angleSpan)
        .def_readwrite("wobble_angle", &pr::WobbleTrajectory::wobbleAngle)
        .def_readwrite("wobble_freq", &pr::WobbleTrajectory::wobbleFreq);

    py::class_<pr::CirclePlusLineTrajectory, AbstractPreparationProtocol>(m,
        "CirclePlusLineTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "source_to_isocenter"_a, "line_length"_a, "start_angle"_a = 0.0_deg,
            "angle_span"_a = 360.0_deg, "fraction_of_views_for_line"_a = 0.5)
        .def("prepare_steps", &pr::CirclePlusLineTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::CirclePlusLineTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("source_to_isocenter", &pr::CirclePlusLineTrajectory::sourceToIsocenter)
        .def_readwrite("line_length", &pr::CirclePlusLineTrajectory::lineLength)
        .def_readwrite("start_angle", &pr::CirclePlusLineTrajectory::startAngle)
        .def_readwrite("angle_span", &pr::CirclePlusLineTrajectory::angleSpan)
        .def_readwrite("fraction_of_views_for_line", &pr::CirclePlusLineTrajectory::fractionOfViewsForLine);

    py::class_<pr::ShortScanTrajectory, AbstractPreparationProtocol>(m,
        "ShortScanTrajectory")
        .def(py::init<double, double, double, bool>(),
            "source_to_isocenter"_a, "start_angle"_a = 0.0_deg, "angle_span"_a = -1.0_deg,
            "rotate_clockwise"_a = false)
        .def("prepare_steps", &pr::ShortScanTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::ShortScanTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("source_to_isocenter", &pr::ShortScanTrajectory::sourceToIsocenter)
        .def_readwrite("start_angle", &pr::ShortScanTrajectory::startAngle)
        .def_readwrite("angle_span", &pr::ShortScanTrajectory::angleSpan)
        .def_readwrite("rotate_clockwise", &pr::ShortScanTrajectory::rotateClockwise);

    py::class_<pr::AxialScanTrajectory, AbstractPreparationProtocol>(m,
        "AxialScanTrajectory")
        .def(py::init<double>(), "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::AxialScanTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::AxialScanTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("start_angle", &pr::AxialScanTrajectory::startAngle);

    py::class_<pr::TomosynthesisCircleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisCircleTrajectory")
        .def(py::init<double, double>(), "source_to_isocenter"_a, "tomo_angle"_a = 15.0_deg)
        .def("prepare_steps", &pr::TomosynthesisCircleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisCircleTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("source_to_isocenter", &pr::TomosynthesisCircleTrajectory::sourceToIsocenter)
        .def_readwrite("tomo_angle", &pr::TomosynthesisCircleTrajectory::tomoAngle);

    py::class_<pr::WedgeTrajectory, AbstractPreparationProtocol>(m, "WedgeTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "tomo_angle"_a = 15.0_deg)
        .def("prepare_steps", &pr::WedgeTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::WedgeTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("start_angle", &pr::WedgeTrajectory::startAngle)
        .def_readwrite("tomo_angle", &pr::WedgeTrajectory::tomoAngle);

    py::class_<pr::TomosynthesisTriangleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisTriangleTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "tomo_angle"_a = 15.0_deg)
        .def("prepare_steps", &pr::TomosynthesisTriangleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisTriangleTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("start_angle", &pr::TomosynthesisTriangleTrajectory::startAngle)
        .def_readwrite("tomo_angle", &pr::TomosynthesisTriangleTrajectory::tomoAngle);

    py::class_<pr::TomosynthesisSaddleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisSaddleTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "tomo_angle"_a = 15.0_deg)
        .def("prepare_steps", &pr::TomosynthesisSaddleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisSaddleTrajectory::isApplicableTo, "setup"_a)
        .def_readwrite("start_angle", &pr::TomosynthesisSaddleTrajectory::startAngle)
        .def_readwrite("tomo_angle", &pr::TomosynthesisSaddleTrajectory::tomoAngle);
}