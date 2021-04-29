#include "pysrc/pybind_common.h"
#include <processing/thresholdvolumesparsifier.h>

namespace py = pybind11;

void init_thresholdvolumesparsifier(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ThresholdVolumeSparsifier, AbstractVolumeSparsifier>(m, "ThresholdVolumeSparsifier")
        .def("sparsify", &ThresholdVolumeSparsifier::sparsify, "volume"_a)
        .def(py::init<float>(), "threshold"_a)
        .def("set_threshold", &ThresholdVolumeSparsifier::setThreshold, "threshold"_a)
        .def("threshold", &ThresholdVolumeSparsifier::threshold);
}
