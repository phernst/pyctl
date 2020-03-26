#include <pybind11/pybind11.h>
#include <img/voxelvolume.h>

namespace py = pybind11;

void init_voxelvolume(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    using VoxelVolumeF = VoxelVolume<float>;

    py::class_<VoxelVolumeF>(m, "VoxelVolumeF")
        .def(py::init([](py::tuple dims)
        {
            return VoxelVolumeF(dims[0].cast<uint>(), dims[1].cast<uint>(), dims[2].cast<uint>());
        }), "nb_voxels"_a)
        .def(py::init([](py::tuple dims, py::tuple sizes)
        {
            return VoxelVolumeF(
                dims[0].cast<uint>(), dims[1].cast<uint>(), dims[2].cast<uint>(),
                sizes[0].cast<float>(), sizes[1].cast<float>(), sizes[2].cast<float>());
        }), "nb_voxels"_a, "voxel_size"_a)
        .def("dimensions", [](const VoxelVolumeF* self)
        {
            const auto& dims = self->dimensions();
            auto pydims { py::tuple(3) };
            pydims[0] = dims.x;
            pydims[1] = dims.y;
            pydims[2] = dims.z;
            return pydims;
        })
        .def("voxel_size", [](const VoxelVolumeF* self)
        {
            const auto& vs = self->voxelSize();
            auto pyvs { py::tuple(3) };
            pyvs[0] = vs.x;
            pyvs[1] = vs.y;
            pyvs[2] = vs.z;
            return pyvs;
        })
        .def("offset", [](const VoxelVolumeF* self)
        {
            const auto& offset = self->offset();
            auto pyoffset { py::tuple(3) };
            pyoffset[0] = offset.x;
            pyoffset[1] = offset.y;
            pyoffset[2] = offset.z;
            return pyoffset;
        })
        .def("has_data", &VoxelVolumeF::hasData)
        .def("total_voxel_count", &VoxelVolumeF::totalVoxelCount)
        .def("max", &VoxelVolumeF::max)
        .def("min", &VoxelVolumeF::min)
        .def("set_volume_offset", [](VoxelVolumeF* self, py::tuple offset)
        {
            self->setVolumeOffset(
                offset[0].cast<float>(),
                offset[1].cast<float>(),
                offset[2].cast<float>());
        }, "offset"_a)
        .def("set_voxel_size", [](VoxelVolumeF* self, py::tuple size)
        {
            self->setVoxelSize(
                size[0].cast<float>(),
                size[1].cast<float>(),
                size[2].cast<float>());
        }, "size"_a)
        .def("fill", &VoxelVolumeF::fill, "fill_value"_a)
        .def_static("ball", &VoxelVolumeF::ball, "radius"_a, "voxel_size"_a, "fill_value"_a)
        .def_static("cube", &VoxelVolumeF::cube, "nb_voxel"_a, "voxel_size"_a, "fill_value"_a)
        .def_static("cylinder_x", &VoxelVolumeF::cylinderX, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a)
        .def_static("cylinder_y", &VoxelVolumeF::cylinderY, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a)
        .def_static("cylinder_z", &VoxelVolumeF::cylinderZ, "radius"_a, "height"_a, "voxel_size"_a,
            "fill_value"_a);
}
