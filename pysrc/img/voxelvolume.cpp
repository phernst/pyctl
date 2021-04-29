#include "pysrc/pybind_common.h"
#include <pybind11/numpy.h>
#include <img/voxelvolume.h>

namespace py = pybind11;

template<typename T>
static py::class_<CTL::VoxelVolume<T>> createPyVoxelVolume(py::module& m, const char* name)
{
    using namespace CTL;
    using namespace py::literals;
    using VV_ = VoxelVolume<T>;

    return py::class_<VoxelVolume<T>>(m, name)
        .def(py::init([](py::tuple dims)
        {
            return VoxelVolume<T>(dims[0].cast<uint>(), dims[1].cast<uint>(), dims[2].cast<uint>());
        }), "nb_voxels"_a)
        .def(py::init([](py::tuple dims, py::tuple sizes)
        {
            return VoxelVolume<T>(
                dims[0].cast<uint>(), dims[1].cast<uint>(), dims[2].cast<uint>(),
                sizes[0].cast<float>(), sizes[1].cast<float>(), sizes[2].cast<float>());
        }), "nb_voxels"_a, "voxel_size"_a)
        .def("dimensions", [](const VoxelVolume<T>& self)
        {
            const auto& dims = self.dimensions();
            auto pydims { py::tuple(3) };
            pydims[0] = dims.x;
            pydims[1] = dims.y;
            pydims[2] = dims.z;
            return pydims;
        })
        .def("voxel_size", [](const VoxelVolume<T>& self)
        {
            const auto& vs = self.voxelSize();
            auto pyvs { py::tuple(3) };
            pyvs[0] = vs.x;
            pyvs[1] = vs.y;
            pyvs[2] = vs.z;
            return pyvs;
        })
        .def("offset", [](const VoxelVolume<T>& self)
        {
            const auto& offset = self.offset();
            auto pyoffset { py::tuple(3) };
            pyoffset[0] = offset.x;
            pyoffset[1] = offset.y;
            pyoffset[2] = offset.z;
            return pyoffset;
        })
        .def("has_data", &VoxelVolume<T>::hasData)
        .def("total_voxel_count", &VoxelVolume<T>::totalVoxelCount)
        .def("max", &VoxelVolume<T>::max)
        .def("min", &VoxelVolume<T>::min)
        .def("set_volume_offset", [](VoxelVolume<T>& self, py::tuple offset)
        {
            self.setVolumeOffset(
                offset[0].cast<float>(),
                offset[1].cast<float>(),
                offset[2].cast<float>());
        }, "offset"_a)
        .def("set_voxel_size", [](VoxelVolume<T>& self, py::tuple size)
        {
            self.setVoxelSize(
                size[0].cast<float>(),
                size[1].cast<float>(),
                size[2].cast<float>());
        }, "size"_a)
        .def("fill", &VoxelVolume<T>::fill, "fill_value"_a)
        .def_static("ball", &VoxelVolume<T>::ball, "radius"_a, "voxel_size"_a, "fill_value"_a)
        .def_static("cube", &VoxelVolume<T>::cube, "nb_voxel"_a, "voxel_size"_a, "fill_value"_a)
        .def_static("cylinder_x", &VoxelVolume<T>::cylinderX, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a)
        .def_static("cylinder_y", &VoxelVolume<T>::cylinderY, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a)
        .def_static("cylinder_z", &VoxelVolume<T>::cylinderZ, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a)
        .def("__iadd__", [](VV_& self, const VV_& rhs) { return self += rhs; }, "other"_a)
        .def("__isub__", [](VV_& self, const VV_& rhs) { return self -= rhs; }, "other"_a)
        .def("__iadd__", [](VV_& self, float s) { return self += s; }, "additive_shift"_a)
        .def("__isub__", [](VV_& self, float s) { return self -= s; }, "subtractive_shift"_a)
        .def("__imul__", [](VV_& self, float s) { return self *= s; }, "factor"_a)
        .def("__itruediv__", [](VV_& self, float s) { return self /= s; }, "divisor"_a)
        .def("__add__", [](const VV_& self, const VV_& rhs) { return self + rhs; }, "rhs"_a)
        .def("__sub__", [](const VV_& self, const VV_& rhs) { return self - rhs; }, "rhs"_a)
        .def("__add__", [](const VV_& self, float s) { return self + s; }, "additive_shift"_a)
        .def("__sub__", [](const VV_& self, float s) { return self - s; }, "subtractive_shift"_a)
        .def("__mul__", [](const VV_& self, float s) { return self*s; }, "factor"_a)
        .def("__truediv__", [](const VV_& self, float s) { return self/s; }, "divisor"_a)
        .def("numpy", [](const VoxelVolume<T>& self) -> py::array_t<T>
        {
            const auto& dims { self.dimensions() };
            return {
                { dims.z, dims.y, dims.x },
                { sizeof(T)*dims.y*dims.x, sizeof(T)*dims.x, sizeof(T) },
                self.rawData()
            };
        }, "Copies the internal data into a new NumPy array of shape [height, width]")
        .def("__getitem__", [](const VoxelVolume<T>& self, py::tuple t)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            const auto z = t[2].cast<uint>();
            return self(x, y, z);
        }, "pos"_a)
        .def("__setitem__", [](VoxelVolume<T>& self, py::tuple t, const T& val)
        {
            const auto x = t[0].cast<uint>();
            const auto y = t[1].cast<uint>();
            const auto z = t[2].cast<uint>();
            self(x, y, z) = val;
        }, "pos"_a, "value"_a)
        .def_static("from_numpy", [](py::array_t<T,py::array::c_style|py::array::forcecast> array)
        -> VoxelVolume<T>
        {
            const auto info { array.request() };
            const auto width { static_cast<uint>(info.shape[2]) };
            const auto height { static_cast<uint>(info.shape[1]) };
            const auto depth { static_cast<uint>(info.shape[0]) };
            const auto fptr { static_cast<T*>(info.ptr) };
            return { width, height, depth, { fptr, fptr+info.size } };
        }, "array"_a);
}

void init_voxelvolume(py::module& m)
{
    createPyVoxelVolume<float>(m, "VoxelVolumeF");
}
