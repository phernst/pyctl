#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <acquisition/preparationprotocols.h>
#include <acquisition/acquisitionsetup.h>

namespace py = pybind11;

void init_preparationprotocols(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    namespace pr = protocols;

    py::class_<pr::FlyingFocalSpot, AbstractPreparationProtocol>(m, "FlyingFocalSpot")
        .def(py::init<std::vector<Vector3x1>, bool>(), "positions"_a, "alternating"_a = false)
        .def_static("two_alternating_spots", &pr::FlyingFocalSpot::twoAlternatingSpots,
            "position1"_a, "position2"_a)
        .def_static("four_alternating_spots", &pr::FlyingFocalSpot::fourAlternatingSpots,
            "position1"_a, "position2"_a, "position3"_a, "position4"_a)
        .def("prepare_steps", &pr::FlyingFocalSpot::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::FlyingFocalSpot::isApplicableTo, "system"_a);

    py::class_<pr::TubeCurrentModulation, AbstractPreparationProtocol>(m, "TubeCurrentModulation")
        .def(py::init<std::vector<double>>(), "currents"_a)
        .def("prepare_steps", &pr::TubeCurrentModulation::prepareSteps, "view_nb"_a, "setup"_a)
        .def("is_applicable_to", &pr::TubeCurrentModulation::isApplicableTo, "system"_a);
}