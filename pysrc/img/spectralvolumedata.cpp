#include <pybind11/pybind11.h>
#include <img/spectralvolumedata.h>

namespace py = pybind11;

void init_spectralvolumedata(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<SpectralVolumeData, VoxelVolume<float>>(m, "SpectralVolumeData")
        .def(py::init<VoxelVolume<float>>(), "mu_values"_a);
}
