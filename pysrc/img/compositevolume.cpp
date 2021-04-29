#include "pysrc/pybind_common.h"
#include <img/compositevolume.h>

namespace py = pybind11;

void init_compositevolume(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<CompositeVolume>(m, "CompositeVolume")
        .def(py::init<>())
        .def(py::init<SpectralVolumeData>(), "volume"_a)
        .def(py::init([](const VoxelVolume<float>& vol)
        {
            return std::unique_ptr<CompositeVolume>(new CompositeVolume(vol));
        }), "volume"_a)
        .def("is_empty", &CompositeVolume::isEmpty)
        .def("mu_volume", &CompositeVolume::muVolume, "vol_idx"_a, "center_energy"_a, "bin_width"_a)
        .def("nb_sub_volumes", &CompositeVolume::nbSubVolumes)
        .def("sub_volume", static_cast<const SpectralVolumeData&(CompositeVolume::*)(uint)const>
            (&CompositeVolume::subVolume), "vol_idx"_a)
        .def("add_sub_volume", static_cast<void(CompositeVolume::*)(SpectralVolumeData)>
            (&CompositeVolume::addSubVolume), "volume"_a)
        .def("add_sub_volume", [](CompositeVolume* self, const VoxelVolume<float>& vol)
        {
            self->addSubVolume(vol);
        }, "volume"_a)
        .def("add_sub_volume", static_cast<void(CompositeVolume::*)(const CompositeVolume&)>
            (&CompositeVolume::addSubVolume), "volume"_a, py::keep_alive<1,2>());
}
