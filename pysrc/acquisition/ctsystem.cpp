#include <pybind11/pybind11.h>
#include <acquisition/ctsystem.h>

namespace py = pybind11;

void init_ctsystem(py::module& m)
{
    using namespace CTL;
    py::class_<CTsystem>(m, "CTsystem")
        .def(py::init<>());
}