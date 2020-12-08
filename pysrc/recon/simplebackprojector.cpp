#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <recon/simplebackprojector.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_simplebackprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using SB_ = OCL::SimpleBackprojector;

    auto sbp = py::class_<SB_, AbstractReconstructor, PySharedPtr<SB_>>(m, "SimpleBackprojector");
    py::enum_<SB_::WeightingType>(sbp, "WeightingType")
        .value("DistanceWeightsOnly", SB_::WeightingType::DistanceWeightsOnly)
        .value("GeometryFactors", SB_::WeightingType::GeometryFactors)
        .value("CustomWeights", SB_::WeightingType::CustomWeights)
        .export_values();

    sbp.def(py::init<SB_::WeightingType>(), "weight_type"_a = SB_::WeightingType::GeometryFactors)
        .def("set_custom_weights", &SB_::setCustomWeights, "weights"_a)
        .def("set_volume_subsampling_factor", &SB_::setVolumeSubsamplingFactor,
            "volume_subsampling_factor"_a)
        .def("set_weighting_type", &SB_::setWeightingType, "weight_type"_a);
}
