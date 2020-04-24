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

    py::class_<SimpleCTSystem, CTSystem>(m, "SimpleCTSystem")
        .def_static("from_ctsystem", [](const CTSystem& system)
        {
            return SimpleCTSystem::fromCTSystem(system);
        })
        .def("detector", &SimpleCTSystem::detector, rvp::reference)
        .def("source", &SimpleCTSystem::source, rvp::reference);
}