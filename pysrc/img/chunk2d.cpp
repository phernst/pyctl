#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <img/chunk2d.h>

namespace py = pybind11;

template<typename T>
static py::class_<CTL::Chunk2D<T>> createPyChunk2D(py::module& m, const char* name)
{
    using namespace CTL;
    using namespace py::literals;

    return py::class_<Chunk2D<T>>(m, name)
        .def(py::init([](py::tuple dims)
        {
            return Chunk2D<T>(dims[0].cast<uint>(), dims[1].cast<uint>());
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
        .def("fill", &Chunk2D<T>::fill, "fill_value"_a);
}

void init_chunk2d(py::module& m)
{
    createPyChunk2D<float>(m, "Chunk2DF");
}
