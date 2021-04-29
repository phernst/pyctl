#include "pysrc/pybind_common.h"
#include <img/sparsevoxelvolume.h>
#include <processing/range.h>
#include "../utils.h"

namespace py = pybind11;

void init_sparsevoxelvolume(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using SVV_ = SparseVoxelVolume;

    auto svv = py::class_<SparseVoxelVolume>(m, "SparseVoxelVolume");
    auto sv = py::class_<SparseVoxelVolume::SingleVoxel>(m, "SingleVoxel");

    svv.def(py::init([](py::tuple voxelSize)
        {
            return SparseVoxelVolume{
                {
                    voxelSize[0].cast<float>(),
                    voxelSize[1].cast<float>(),
                    voxelSize[2].cast<float>()
                }
            };
        }), "voxel_size"_a)
        .def(py::init([](py::tuple voxelSize, std::vector<SVV_::SingleVoxel> data)
        {
            return SparseVoxelVolume{
                {
                    voxelSize[0].cast<float>(),
                    voxelSize[1].cast<float>(),
                    voxelSize[2].cast<float>()
                },
                data
            };
        }), "voxel_size"_a, "data"_a)
        .def("add_voxel", static_cast<void(SVV_::*)(const SVV_::SingleVoxel&)>(&SVV_::addVoxel),
            "voxel"_a)
        .def("add_voxel", static_cast<void(SVV_::*)(float,float,float,float)>(&SVV_::addVoxel),
            "x"_a, "y"_a, "z"_a, "val"_a)
        .def("bounding_box", &SVV_::boundingBox)
        .def("data", static_cast<std::vector<SVV_::SingleVoxel>&(SVV_::*)()>(&SVV_::data))
        .def("nb_voxels", &SVV_::nbVoxels)
        .def("remove_voxel", &SVV_::removeVoxel, "i"_a)
        .def("sparsity_level", static_cast<float(SVV_::*)()const>(&SVV_::sparsityLevel))
        .def("sparsity_level", [](const SVV_& self, py::tuple referenceDimension)
        {
            return self.sparsityLevel({
                referenceDimension[0].cast<uint>(),
                referenceDimension[1].cast<uint>(),
                referenceDimension[2].cast<uint>()
            });
        }, "reference_dimension"_a)
        .def("voxel", static_cast<SVV_::SingleVoxel&(SVV_::*)(uint)>(&SVV_::voxel), "i"_a)
        .def("voxel_size", [](const SVV_& self)
        {
            const auto& dims = self.voxelSize();
            auto pydims { py::tuple(3) };
            pydims[0] = dims.x;
            pydims[1] = dims.y;
            pydims[2] = dims.z;
            return pydims;
        })
        .def("paint_to_voxel_volume", &SVV_::paintToVoxelVolume, "volume"_a)
        .def("to_voxel_volume", static_cast<VoxelVolume<float>(SVV_::*)()const>
            (&SVV_::toVoxelVolume))
        .def("to_voxel_volume", [](const SVV_& self, py::tuple offset)
        {
            return self.toVoxelVolume(
                SVV_::Offset{
                    offset[0].cast<float>(),
                    offset[1].cast<float>(),
                    offset[2].cast<float>()
                }
            );
        }, "offset"_a)
        .def("to_voxel_volume", [](const SVV_& self, py::tuple dimension, py::tuple offset)
        {
            return self.toVoxelVolume(
                SVV_::Dimensions{
                    dimension[0].cast<uint>(),
                    dimension[1].cast<uint>(),
                    dimension[2].cast<uint>()
                },
                SVV_::Offset{
                    offset[0].cast<float>(),
                    offset[1].cast<float>(),
                    offset[2].cast<float>()
                }
            );
        }, "dimension"_a, "offset"_a = zero_tuple<float,3>());

    sv.def(py::init<float,float,float,float>(), "x"_a, "y"_a, "z"_a, "val"_a)
        .def("x", &SVV_::SingleVoxel::x)
        .def("y", &SVV_::SingleVoxel::y)
        .def("z", &SVV_::SingleVoxel::z)
        .def_property("value", static_cast<float(SVV_::SingleVoxel::*)()const> // TODO
            (&SVV_::SingleVoxel::value), [](SVV_::SingleVoxel& self, float f)
            {
                self.value() = f;
            });
}
