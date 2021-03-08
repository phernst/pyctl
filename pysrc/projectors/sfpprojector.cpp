#include <pybind11/pybind11.h>
#include <img/sparsevoxelvolume.h>
#include <projectors/sfpprojector.h>
#include <acquisition/acquisitionsetup.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_sfpprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto sfpp = py::class_<OCL::SFPProjector, AbstractProjector, PySharedPtr<OCL::SFPProjector>>(m, "SFPProjector")
        .def(py::init<>())
        .def("configure", &OCL::SFPProjector::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &OCL::SFPProjector::project, "volume"_a)
        .def("project_sparse", &OCL::SFPProjector::projectSparse, "volume"_a)
        .def("settings", &OCL::SFPProjector::settings);

    py::enum_<OCL::SFPProjector::FootprintType>(sfpp, "FootprintType")
        .value("TR", OCL::SFPProjector::TR)
        .value("TT", OCL::SFPProjector::TT)
        .value("TT_Generic", OCL::SFPProjector::TT_Generic)
        .export_values();

    py::class_<OCL::SFPProjector::Settings>(sfpp, "Settings")
        .def(py::init<>())
        .def_readwrite("footprint_type", &OCL::SFPProjector::Settings::footprintType)
        .def_readwrite("use_atomic_float", &OCL::SFPProjector::Settings::useAtomicFloat);
}
