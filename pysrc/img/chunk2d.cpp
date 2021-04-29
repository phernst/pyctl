#include "pysrc/pybind_common.h"
#include <pybind11/numpy.h>
#include <img/chunk2d.h>

namespace py = pybind11;

template<typename T>
static py::class_<CTL::Chunk2D<T>> createPyChunk2D(py::module& m, const char* name)
{
    using namespace CTL;
    using namespace py::literals;
    using Ch_ = Chunk2D<T>;

    return py::class_<Ch_>(m, name)
        .def(py::init([](py::tuple dims) -> Ch_
        {
            return { dims[0].cast<uint>(), dims[1].cast<uint>() };
        }), "dimensions"_a)
        .def(py::init<uint, uint>(), "width"_a, "height"_a)
        .def("dimensions", [](const Ch_* self)
        {
            const auto& dims = self->dimensions();
            auto pydims { py::tuple(2) };
            pydims[0] = dims.width;
            pydims[1] = dims.height;
            return pydims;
        })
        .def("max", &Ch_::max)
        .def("min", &Ch_::min)
        .def("data", static_cast<std::vector<T>&(Ch_::*)()>(&Ch_::data))
        .def("height", &Ch_::height)
        .def("width", &Ch_::width)
        .def("fill", &Ch_::fill, "fill_value"_a)
        .def("__eq__", [](const Ch_& self, const Ch_& rhs) { return self == rhs; }, "other"_a)
        .def("__ne__", [](const Ch_& self, const Ch_& rhs) { return self != rhs; }, "other"_a)
        .def("__iadd__", [](Ch_& self, const Ch_& rhs) { return self += rhs; }, "rhs"_a)
        .def("__isub__", [](Ch_& self, const Ch_& rhs) { return self -= rhs; }, "rhs"_a)
        .def("__imul__", [](Ch_& self, const T& s) { return self *= s; }, "factor"_a)
        .def("__itruediv__", [](Ch_& self, const T& s) { return self /= s; }, "divisor"_a)
        .def("__add__", [](const Ch_& self, const Ch_& rhs) { return self + rhs; }, "rhs"_a)
        .def("__sub__", [](const Ch_& self, const Ch_& rhs) { return self - rhs; }, "rhs"_a)
        .def("__mul__", [](const Ch_& self, const T& s) { return self*s; }, "factor"_a)
        .def("__truediv__", [](const Ch_& self, const T& s) { return self/s; }, "divisor"_a)
        .def("numpy", [](const Ch_& self) -> py::array_t<T>
        {
            return {
                { self.height(), self.width() },
                { sizeof(T)*self.width(), sizeof(T) },
                self.rawData()
            };
        }, "Copies the internal data into a new NumPy array of shape [height, width]")
        .def("__getitem__", [](const Ch_& self, py::tuple t)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            return self(x, y);
        }, "pos"_a)
        .def("__setitem__", [](Ch_& self, py::tuple t, const T& val)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            self(x, y) = val;
        }, "pos"_a, "value"_a)
        .def_static("from_numpy", [](py::array_t<T,py::array::c_style|py::array::forcecast> array)
        -> Ch_
        {
            const auto info { array.request() };
            const auto width { static_cast<uint>(info.shape[1]) };
            const auto height { static_cast<uint>(info.shape[0]) };
            const auto fptr { static_cast<T*>(info.ptr) };
            return { width, height, { fptr, fptr+info.size } };
        }, "array"_a);
}

void init_chunk2d(py::module& m)
{
    createPyChunk2D<float>(m, "Chunk2DF");
}
