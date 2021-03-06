#include "pysrc/pybind_common.h"
#include <pybind11/numpy.h>
#include <mat/matrix.h>
#include <sstream>

namespace py = pybind11;

template<uint Rows, uint Cols>
static py::class_<CTL::mat::Matrix<Rows,Cols>> createPyMatrix(py::module& m, const char* name)
{
    using namespace CTL::mat;
    using namespace py::literals;
    using rvp = py::return_value_policy;
    using Mat_ = Matrix<Rows,Cols>;

    return py::class_<Mat_>(m, name, py::buffer_protocol())
        .def_buffer([](Mat_& m) -> py::buffer_info
        {
            return {
                m.data(),
                sizeof(double),
                py::format_descriptor<double>::format(),
                2,
                { Rows, Cols },
                { sizeof(double)*Cols, sizeof(double) }
            };
        })
        .def(py::init([](py::array_t<double,py::array::c_style|py::array::forcecast> np)
        {
            auto info { np.request() };

            if (info.ndim != 2)
            {
                throw std::runtime_error("Incompatible buffer dimensions!");
            }

            auto result { Mat_() };
            std::copy_n(static_cast<double*>(info.ptr), Rows*Cols, result.data());
            return result;
        }), "init_array")
        .def("__neg__", [](const Mat_& self) { return -self; })
        .def("__imul__", [](Mat_& self, double s) { return self *= s; }, "scalar"_a)
        .def("__itruediv__", [](Mat_& self, double s) { return self /= s; }, "scalar"_a)
        .def("__iadd__", [](Mat_& self, const Mat_& rhs) { return self += rhs; }, "rhs"_a)
        .def("__isub__", [](Mat_& self, const Mat_& rhs) { return self -= rhs; }, "rhs"_a)
        .def("__imul__", [](Mat_& self, const Mat_& rhs) { return self %= rhs; }, "rhs"_a)
        .def("__itruediv__", [](Mat_& self, const Mat_& rhs) { return self |= rhs; }, "rhs"_a)
        .def("__mul__", [](const Mat_& self, double s) { return self*s; }, "scalar"_a)
        .def("__truediv__", [](const Mat_& self, double s) { return self/s; }, "scalar"_a)
        .def("__add__", [](const Mat_& self, const Mat_& rhs) { return self + rhs; }, "rhs"_a)
        .def("__sub__", [](const Mat_& self, const Mat_& rhs) { return self - rhs; }, "rhs"_a)
        .def("__mul__", [](const Mat_& self, const Mat_& rhs) { return self%rhs; }, "rhs"_a)
        .def("__truediv__", [](const Mat_& self, const Mat_& rhs) { return self|rhs; }, "rhs"_a)
        .def("__repr__", [](const Mat_& self)
        {
            auto repr { std::stringstream() };
            const auto pre { std::string("Matrix([") };
            repr << pre;
            for (auto i {0u}; i < Rows - 1; i++)
            {
                repr << "[";
                for (auto j {0u}; j < Cols - 1; j++)
                {
                    repr << self.at(i, j) << ", ";
                }
                repr << self.at(i, Cols - 1) << "],\n";
                for (auto i {0u}; i < pre.length(); i++) repr << " ";
            }
            repr << "[";
            for (auto j {0u}; j < Cols - 1; j++)
            {
                repr << self.at(Rows - 1, j) << ", ";
            }
            repr << self.at(Rows - 1, Cols - 1) << "]])";
            return repr.str();
        });
}

void init_matrix(py::module& m)
{
    createPyMatrix<3,3>(m, "Matrix3x3");
    createPyMatrix<3,4>(m, "Matrix3x4"); // needed for ProjectionMatrix
    createPyMatrix<3,1>(m, "Vector3x1");
    createPyMatrix<2,1>(m, "Vector2x1"); // needed for ProjectionMatrix
    
    py::implicitly_convertible<
        py::array_t<double,py::array::c_style|py::array::forcecast>,
        CTL::mat::Matrix<3,3>>();

    py::implicitly_convertible<
        py::array_t<double,py::array::c_style|py::array::forcecast>,
        CTL::mat::Matrix<3,4>>();

    py::implicitly_convertible<
        py::array_t<double,py::array::c_style|py::array::forcecast>,
        CTL::mat::Matrix<3,1>>();
    
    py::implicitly_convertible<
        py::array_t<double,py::array::c_style|py::array::forcecast>,
        CTL::mat::Matrix<2,1>>();
}
