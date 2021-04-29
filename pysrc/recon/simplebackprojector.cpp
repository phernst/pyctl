#include "pysrc/pybind_common.h"
#include <recon/simplebackprojector.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_simplebackprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using SB_ = OCL::SimpleBackprojector;
    using BB_ = OCL::BackprojectorBase;

    auto sbp = py::class_<SB_, BB_, PySharedPtr<SB_>>(m, "SimpleBackprojector");

    sbp.def(py::init<BB_::WeightingType>(), "weight_type"_a = BB_::WeightingType::GeometryFactors)
        .def("set_custom_weights", &SB_::setCustomWeights, "weights"_a)
        .def("set_volume_subsampling_factor", &SB_::setVolumeSubsamplingFactor,
            "volume_subsampling_factor"_a);
}
