#include <pybind11/pybind11.h>
#include <recon/sfpbackprojector.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_sfpbackprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using SFPB_ = OCL::SFPBackprojector;

    py::class_<SFPB_, AbstractReconstructor, PySharedPtr<SFPB_>>(m, "SFPBackprojector")
        .def(py::init<>());
}
