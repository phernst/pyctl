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
        .def("set_detector_displacement_angles", &AbstractGantry::setDetectorDisplacementAngles,
            "roll_angle"_a, "tilt_angle"_a, "twist_angle"_a);
}
