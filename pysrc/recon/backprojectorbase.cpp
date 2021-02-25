#include <pybind11/pybind11.h>
#include <img/voxelvolume.h>
#include <recon/backprojectorbase.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_backprojectorbase(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using BB_ = OCL::BackprojectorBase;

    auto bpb = py::class_<BB_, AbstractReconstructor, PySharedPtr<BB_>>(m, "BackprojectorBase");
    py::enum_<BB_::WeightingType>(bpb, "WeightingType")
        .value("DistanceWeightsOnly", BB_::WeightingType::DistanceWeightsOnly)
        .value("GeometryFactors", BB_::WeightingType::GeometryFactors);

    bpb.def("reconstruct_to_plain", &BB_::reconstructToPlain, "projections"_a, 
            "target_volume"_a)
        .def("is_config_consistent_with", &BB_::isConfigConsistentWith,
            "projections"_a)
        .def("setup", &BB_::setup)
        .def("weighting_type", &BB_::weightingType)
        .def("set_weighting_type", &BB_::setWeightingType, "weight_type");
}
