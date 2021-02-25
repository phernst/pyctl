#include <pybind11/pybind11.h>
#include <recon/fdkreconstructor.h>
#include <img/voxelvolume.h>
#include <processing/genericoclprojectionfilter.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_fdkreconstructor(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<OCL::FDKReconstructor, AbstractReconstructor, PySharedPtr<OCL::FDKReconstructor>>(m, "FDKReconstructor")
        .def(py::init<float>(), "q"_a = 1.0f)
        .def(py::init([](PySharedPtr<OCL::GenericOCLProjectionFilter> pf, float q)
        {
            return make_pysharedptr<OCL::FDKReconstructor>(
                pf.release_to_ctl(), q
            );
        }), "projection_filter"_a, "q"_a = 1.0f)
        .def("set_revised_parker_weight_q", &OCL::FDKReconstructor::setRevisedParkerWeightQ, "q"_a)
        .def("use_sfp_backprojector", &OCL::FDKReconstructor::useSFPBackprojector)
        .def("use_simple_backprojector", &OCL::FDKReconstructor::useSimpleBackrojector)
        .def("is_sfp_backprojector_in_use", &OCL::FDKReconstructor::isSFPBackprojectorInUse)
        .def("is_simple_backprojector_in_use", &OCL::FDKReconstructor::isSimpleBackprojectorInUse)
        .def("revised_parker_weight_q", &OCL::FDKReconstructor::revisedParkerWeightQ)
        .def("is_applicable_to", &OCL::FDKReconstructor::isApplicableTo, "setup"_a);
}
