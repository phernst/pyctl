#include "pysrc/pybind_common.h"
#include <processing/genericoclprojectionfilter.h>
#include <img/projectiondata.h>
#include <vector>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_genericoclprojectionfilter(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using GPF = OCL::GenericOCLProjectionFilter;

    py::class_<GPF, AbstractProjectionFilter, PySharedPtr<GPF>>(m, "GenericOCLProjectionFilter")
        .def("filter", &GPF::filter, "projections"_a)
        .def(py::init<const std::string&, const std::vector<float>>(),
            "cl_file_name"_a, "arguments"_a = std::vector<float>{})
        .def("set_additional_kernel_arg", &GPF::setAdditionalKernelArg,
            "argument"_a)
        .def("set_additional_kernel_args", &GPF::setAdditionalKernelArgs,
            "arguments"_a);
}
