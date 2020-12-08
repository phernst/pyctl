#include <pybind11/pybind11.h>
#include <processing/abstractvolumesparsifier.h>

namespace py = pybind11;

void init_abstractvolumesparsifier(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractVolumeSparsifier>(m, "AbstractVolumeSparsifier")
        .def("sparsify", &AbstractVolumeSparsifier::sparsify, "volume"_a);
}
