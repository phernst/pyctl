#include <pybind11/pybind11.h>
#include <img/singleviewdata.h>

namespace py = pybind11;

void init_singleviewdata(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<SingleViewData>(m, "SingleViewData")
        .def(py::init([](py::tuple dims)
        {
            return SingleViewData(dims[0].cast<uint>(), dims[1].cast<uint>());
        }), "dimensions"_a)
        .def(py::init([](const Chunk2D<float> data)
        {
            return SingleViewData(data);
        }), "module_data"_a)
        .def(py::init<uint, uint>(), "channels_per_module"_a, "rows_per_module"_a)
        .def("dimensions", [](const SingleViewData* self)
        {
            const auto& dims = self->dimensions();
            auto pydims { py::tuple(3) };
            pydims[0] = dims.nbChannels;
            pydims[1] = dims.nbRows;
            pydims[2] = dims.nbModules;
            return pydims;
        })
        .def("max", &SingleViewData::max)
        .def("min", &SingleViewData::min)
        .def("module", static_cast<Chunk2D<float>&(SingleViewData::*)(uint)>
            (&SingleViewData::module), "i"_a)
        .def("elements_per_module", &SingleViewData::elementsPerModule)
        .def("nb_modules", &SingleViewData::nbModules)
        .def("total_pixel_count", &SingleViewData::totalPixelCount)
        .def("append", static_cast<void(SingleViewData::*)(const Chunk2D<float>&)>
            (&SingleViewData::append), "module_data"_a)
        .def("fill", &SingleViewData::fill, "fill_value"_a)
        .def("max", &SingleViewData::max)
        .def("min", &SingleViewData::min)
        .def("transform_to_extinction", &SingleViewData::transformToExtinction, "i0_or_N0"_a = 1.0)
        .def("transform_to_intensity", &SingleViewData::transformToIntensity, "i0"_a = 1.0)
        .def("transform_to_counts", &SingleViewData::transformToCounts, "N0"_a = 1.0);
}
