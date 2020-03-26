#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <acquisition/ctsystem.h>
#include <components/abstractdetector.h>
#include <components/abstractsource.h>

namespace py = pybind11;

void init_ctsystem(py::module& m)
{
    using namespace CTL;
    py::class_<CTsystem>(m, "CTsystem")
        .def(py::init<>())
        .def("detectors", &CTsystem::detectors, py::return_value_policy::reference)
        .def("sources", &CTsystem::sources, py::return_value_policy::reference);
}
