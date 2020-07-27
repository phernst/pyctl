#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
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
        .def("set_angle_increment", &pr::HelicalTrajectory::setAngleIncrement, "angle_increment"_a)
        .def("set_pitch_increment", &pr::HelicalTrajectory::setPitchIncrement, "pitch_increment"_a)
        .def("set_start_pitch", &pr::HelicalTrajectory::setStartPitch, "start_pitch"_a)
        .def("set_start_angle", &pr::HelicalTrajectory::setStartAngle, "start_angle"_a);

    py::class_<pr::WobbleTrajectory, AbstractPreparationProtocol>(m, "WobbleTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "start_angle"_a = 0.0_deg,
            "wobble_angle"_a = 15.0_deg, "wobble_freq"_a = 1.0)
        .def("prepare_steps", &pr::WobbleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::WobbleTrajectory::isApplicableTo, "setup"_a)
        .def("set_start_angle", &pr::WobbleTrajectory::setStartAngle, "start_angle"_a)
        .def("set_wobble_angle", &pr::WobbleTrajectory::setWobbleAngle, "wobble_angle"_a)
        .def("set_wobble_freq", &pr::WobbleTrajectory::setWobbleFreq, "wobble_freq"_a);

    py::class_<pr::CirclePlusLineTrajectory, AbstractPreparationProtocol>(m,
        "CirclePlusLineTrajectory")
        .def(py::init<double, double, double, double, double>(),
            "angle_span"_a, "source_to_isocenter"_a, "line_length"_a,
            "fraction_of_views_for_line"_a = 0.5, "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::CirclePlusLineTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::CirclePlusLineTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::ShortScanTrajectory, AbstractPreparationProtocol>(m,
        "ShortScanTrajectory")
        .def(py::init<double, double, double>(),
            "source_to_isocenter"_a, "start_angle"_a = 0.0_deg, "angle_span"_a = -1.0_deg)
        .def("prepare_steps", &pr::ShortScanTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::ShortScanTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::AxialScanTrajectory, AbstractPreparationProtocol>(m,
        "AxialScanTrajectory")
        .def(py::init<double>(), "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::AxialScanTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::AxialScanTrajectory::isApplicableTo, "setup"_a)
        .def("set_start_angle", &pr::AxialScanTrajectory::setStartAngle, "start_angle"_a);

    py::class_<pr::TomosynthesisCircleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisCircleTrajectory")
        .def(py::init<double, double>(), "source_to_isocenter"_a, "tomo_angle"_a = 15.0_deg)
        .def("prepare_steps", &pr::TomosynthesisCircleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisCircleTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::WedgeTrajectory, AbstractPreparationProtocol>(m, "WedgeTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "tomo_angle"_a = 15.0_deg,
            "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::WedgeTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::WedgeTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::TomosynthesisTriangleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisTriangleTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "tomo_angle"_a = 15.0_deg,
            "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::TomosynthesisTriangleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisTriangleTrajectory::isApplicableTo, "setup"_a);

    py::class_<pr::TomosynthesisSaddleTrajectory, AbstractPreparationProtocol>(m,
        "TomosynthesisSaddleTrajectory")
        .def(py::init<double, double, double>(), "source_to_isocenter"_a, "tomo_angle"_a = 15.0_deg,
            "start_angle"_a = 0.0_deg)
        .def("prepare_steps", &pr::TomosynthesisSaddleTrajectory::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TomosynthesisSaddleTrajectory::isApplicableTo, "setup"_a);
}