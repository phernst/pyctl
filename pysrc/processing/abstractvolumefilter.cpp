#include <pybind11/pybind11.h>
#include <processing/abstractvolumefilter.h>
#include <img/voxelvolume.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_abstractvolumefilter(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractVolumeFilter, PySharedPtr<AbstractVolumeFilter>>(m, "AbstractVolumeFilter")
        .def("filter", &AbstractVolumeFilter::filter, "volume"_a);
}
