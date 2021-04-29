#include "pysrc/pybind_common.h"
#include <pybind11/numpy.h>
#include <mat/matrix_utils.h>
#include <sstream>

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
        .def_readwrite("rotation", &mat::Location::rotation)
        .def("__repr__", [](const mat::Location& self)
        {
            auto repr { std::stringstream() };
            const auto pre { std::string("Location(") };
            const auto pos { std::string("position: ") };
            const auto rot { std::string("rotation: ") };
            repr << pre;
            repr << pos;
            auto vecrep { py::module::import("ctl").attr("mat").attr("Vector3x1").attr("__repr__") };
            auto ispos { std::istringstream(vecrep(self.position).cast<std::string>()) };
            {
                auto line { std::string() };
                std::getline(ispos, line);
                repr << line << '\n';
            }
            auto poslines { std::vector<std::string>() };
            {
                for (auto line { std::string() }; std::getline(ispos, line); )
                    poslines.push_back(line);
            }
            for (auto i {0u}; i < poslines.size() - 1; i++)
            {
                for (auto j { 0u }; j < pre.length() + pos.length(); j++) repr << ' ';
                repr << poslines[i] << '\n';
            }
            for (auto j { 0u }; j < pre.length() + pos.length(); j++) repr << ' ';
            repr << poslines[poslines.size() - 1] << ",\n";

            for (auto i {0u}; i < pre.length(); i++) repr << ' ';
            repr << rot;

            auto matrep { py::module::import("ctl").attr("mat").attr("Matrix3x3").attr("__repr__") };
            auto isrot { std::istringstream(matrep(self.rotation).cast<std::string>()) };
            {
                auto line { std::string() };
                std::getline(isrot, line);
                repr << line << '\n';
            }
            auto matlines { std::vector<std::string>() };
            {
                for (auto line { std::string() }; std::getline(isrot, line); )
                    matlines.push_back(line);
            }
            for (auto i {0u}; i < matlines.size() - 1; i++)
            {
                for (auto j { 0u }; j < pre.length() + rot.length(); j++) repr << ' ';
                repr << matlines[i] << '\n';
            }
            for (auto j { 0u }; j < pre.length() + rot.length(); j++) repr << ' ';
            repr << matlines[matlines.size() - 1] << ')';
            return repr.str();
        });
}
