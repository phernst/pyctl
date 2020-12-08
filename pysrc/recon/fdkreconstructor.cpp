#include <pybind11/pybind11.h>
#include <recon/fdkreconstructor.h>
#include <img/voxelvolume.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_fdkreconstructor(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<OCL::FDKReconstructor, AbstractReconstructor, PySharedPtr<OCL::FDKReconstructor>>(m, "FDKReconstructor")
        .def(py::init<float>(), "q"_a = 1.0f)
        .def("set_revised_parker_weight_q", &OCL::FDKReconstructor::setRevisedParkerWeightQ, "q"_a)
        .def("is_applicable_to", &OCL::FDKReconstructor::isApplicableTo, "setup"_a);
}
