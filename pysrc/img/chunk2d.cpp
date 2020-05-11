#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <img/chunk2d.h>

namespace py = pybind11;

template<typename T>
static py::class_<CTL::Chunk2D<T>> createPyChunk2D(py::module& m, const char* name)
{
    using namespace CTL;
    using namespace py::literals;

    return py::class_<Chunk2D<T>>(m, name)
        .def(py::init([](py::tuple dims) -> Chunk2D<T>
        {
            return { dims[0].cast<uint>(), dims[1].cast<uint>() };
        }), "dimensions"_a)
        .def(py::init<uint, uint>(), "width"_a, "height"_a)
        .def("dimensions", [](const Chunk2D<T>* self)
        {
            const auto& dims = self->dimensions();
            auto pydims { py::tuple(2) };
            pydims[0] = dims.width;
            pydims[1] = dims.height;
            return pydims;
        })
        .def("max", &Chunk2D<T>::max)
        .def("min", &Chunk2D<T>::min)
        .def("data", static_cast<std::vector<T>&(Chunk2D<T>::*)()>(&Chunk2D<T>::data))
        .def("height", &Chunk2D<T>::height)
        .def("width", &Chunk2D<T>::width)
        .def("fill", &Chunk2D<T>::fill, "fill_value"_a)
        .def("numpy", [](const Chunk2D<T>& self) -> py::array_t<T>
        {
            return {
                { self.height(), self.width() },
                { sizeof(T)*self.width(), sizeof(T) },
                self.rawData()
            };
        }, "Copies the internal data into a new NumPy array of shape [height, width]")
        .def("__getitem__", [](const Chunk2D<T>& self, py::tuple t)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            return self(x, y);
        }, "pos"_a)
        .def("__setitem__", [](Chunk2D<T>& self, py::tuple t, const T& val)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            self(x, y) = val;
        }, "pos"_a, "value"_a)
        .def_static("from_numpy", [](py::array_t<T,py::array::c_style|py::array::forcecast> array)
        -> Chunk2D<T>
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
