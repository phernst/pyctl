#include <pybind11/pybind11.h>
#include <acquisition/simplectsystem.h>
#include <components/abstractdetector.h>
#include <components/abstractsource.h>

namespace py = pybind11;

void init_simplectsystem(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<SimpleCTsystem, CTsystem>(m, "SimpleCTsystem")
        .def_static("from_ctsystem", [](const CTsystem& system)
        {
            return SimpleCTsystem::fromCTsystem(system);
        })
        .def("detector", &SimpleCTsystem::detector, rvp::reference)
        .def("source", &SimpleCTsystem::source, rvp::reference);
}