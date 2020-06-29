#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <img/projectiondata.h>

namespace py = pybind11;

void init_projectiondata(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ProjectionData>(m, "ProjectionData")
        .def(py::init([](py::tuple dims)
        {
            return ProjectionData(dims[0].cast<uint>(), dims[1].cast<uint>(), dims[2].cast<uint>());
        }), "dimensions"_a)
        .def(py::init<uint, uint, uint>(),
            "channels_per_module"_a, "rows_per_module"_a, "nb_modules"_a)
        .def(py::init([](const SingleViewData& data)
        {
            return ProjectionData(data);
        }), "single_view_data"_a)
        .def("dimensions", [](const ProjectionData* self)
        {
            const auto& dims = self->dimensions();
            auto pydims { py::tuple(4) };
            pydims[0] = dims.nbChannels;
            pydims[1] = dims.nbRows;
            pydims[2] = dims.nbModules;
            pydims[3] = dims.nbViews;
            return pydims;
        })
        .def("max", &ProjectionData::max)
        .def("min", &ProjectionData::min)
        .def("to_vector", &ProjectionData::toVector)
        .def("nb_views", &ProjectionData::nbViews)
        .def("view", static_cast<SingleViewData&(ProjectionData::*)(uint)>
            (&ProjectionData::view), "i"_a)
        .def("view_dimensions", &ProjectionData::viewDimensions)
        .def("append", static_cast<void(ProjectionData::*)(const SingleViewData&)>
            (&ProjectionData::append), "single_view"_a, py::keep_alive<1,2>())
        .def("combined", &ProjectionData::combined, "layout"_a = ModuleLayout())
        .def("fill", &ProjectionData::fill, "fill_value"_a)
        .def("max", &ProjectionData::max)
        .def("min", &ProjectionData::min)
        .def("transform_to_extinction", static_cast<void(ProjectionData::*)(double)>
            (&ProjectionData::transformToExtinction), "i0_or_N0"_a = 1.0)
        .def("transform_to_intensity", static_cast<void(ProjectionData::*)(double)>
            (&ProjectionData::transformToIntensity), "i0"_a = 1.0)
        .def("transform_to_counts", static_cast<void(ProjectionData::*)(double)>
            (&ProjectionData::transformToCounts), "N0"_a = 1.0)
        .def("numpy", [](const ProjectionData& self) -> py::array_t<float>
        {
            const auto& dims { self.dimensions() };
            const auto stride_views { sizeof(float)*dims.nbModules*dims.nbRows*dims.nbChannels };
            const auto stride_modules { sizeof(float)*dims.nbRows*dims.nbChannels };
            const auto stride_rows { sizeof(float)*dims.nbChannels };
            const auto stride_cols { sizeof(float)*1u };
            const auto vec { self.toVector() };
            return {
                { dims.nbViews, dims.nbModules, dims.nbRows, dims.nbChannels },
                { stride_views, stride_modules, stride_rows, stride_cols },
                vec.data()
            };
        }, "Copies the internal data into a NumPy array of shape [views, modules, rows, channels]");
}
