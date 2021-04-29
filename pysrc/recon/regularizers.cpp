#include "pysrc/pybind_common.h"
#include <recon/regularizers.h>
#include <img/voxelvolume.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_regularizers(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<IdentityRegularizer, AbstractVolumeFilter, PySharedPtr<IdentityRegularizer>>(m, "IdentityRegularizer")
        .def("filter", &IdentityRegularizer::filter, "volume"_a)
        .def(py::init<>());

    auto mfr = py::class_<MedianFilterRegularizer, AbstractVolumeFilter, PySharedPtr<MedianFilterRegularizer>>(m, "MedianFilterRegularizer");

    py::enum_<MedianFilterRegularizer::NeighborHood>(mfr, "NeighborHood")
        .value("Box3x3x3", MedianFilterRegularizer::Box3x3x3)
        .value("NearestOnly", MedianFilterRegularizer::NearestOnly)
        .export_values();

    mfr.def("filter", &MedianFilterRegularizer::filter, "volume"_a)
        .def(py::init<float, MedianFilterRegularizer::NeighborHood>(),
            "strength"_a = 0.1f, "neighbors"_a = MedianFilterRegularizer::NearestOnly)
        .def("set_neighbor_hood_type", &MedianFilterRegularizer::setNeighborHoodType,
            "neighbors"_a)
        .def("set_regularization_strength", &MedianFilterRegularizer::setRegularizationStrength,
            "strength"_a);

    auto tvr = py::class_<TVRegularizer, AbstractVolumeFilter, PySharedPtr<TVRegularizer>>(m, "TVRegularizer");

    py::enum_<TVRegularizer::NeighborHood>(tvr, "NeighborHood")
        .value("Box3x3x3", TVRegularizer::Box3x3x3)
        .value("NearestOnly", TVRegularizer::NearestOnly)
        .export_values();

    tvr.def("filter", &TVRegularizer::filter, "volume"_a)
        .def(py::init<float, TVRegularizer::NeighborHood>(),
            "max_change_in_10hu"_a = 1.0f, "neighbors"_a = TVRegularizer::NearestOnly)
        .def("set_neighbor_hood_type", &TVRegularizer::setNeighborHoodType,
            "neighbors"_a)
        .def("set_regularization_strength", &TVRegularizer::setRegularizationStrength,
            "max_change_in_10hu"_a);

    py::class_<HuberRegularizer, AbstractVolumeFilter, PySharedPtr<HuberRegularizer>>(m, "HuberRegularizer")
        .def("filter", &HuberRegularizer::filter, "volume"_a)
        .def(py::init<float, float, float, float, float>(), "strength"_a = 1.0f,
            "huber_edge_in_100hu"_a = 1.0f, "weight_z"_a = 1.0f,
            "weight_xy"_a = 1.0f, "direct_z_weight"_a = 1.0f)
        .def("set_regularization_strength", &HuberRegularizer::setRegularizationStrength,
            "strength"_a)
        .def("set_huber_edge", &HuberRegularizer::setHuberEdge,
            "edge_in_100hu"_a)
        .def("set_relative_weighting", &HuberRegularizer::setRelativeWeighting,
            "weight_z"_a, "weight_xy"_a = 1.0f)
        .def("set_direct_z_neighbor_weight", &HuberRegularizer::setDirectZNeighborWeight,
            "weight"_a);
}
