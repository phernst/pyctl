#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <mat/matrix_utils.h>

namespace py = pybind11;

void init_matrix_utils(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<mat::Location>(m, "Location")
        .def(py::init())
        .def(py::init<const Vector3x1&, const Matrix3x3&>(), "pos"_a, "rot"_a)
        .def_readwrite("position", &mat::Location::position)
        .def_readwrite("location", &mat::Location::rotation);
}
