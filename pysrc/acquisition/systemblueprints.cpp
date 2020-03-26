#include <pybind11/pybind11.h>
#include <acquisition/systemblueprints.h>

namespace py = pybind11;

void init_systemblueprints(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::enum_<DetectorBinning>(m, "DetectorBinning")
        .value("Binning1x1", DetectorBinning::Binning1x1)
        .value("Binning2x2", DetectorBinning::Binning2x2)
        .value("Binning4x4", DetectorBinning::Binning4x4);

    auto bp { m.def_submodule("blueprints") };
    py::class_<blueprints::GenericTubularCT, AbstractCTsystemBlueprint>(bp, "GenericTubularCT")
        .def(py::init<>());
    py::class_<blueprints::GenericCarmCT, AbstractCTsystemBlueprint>(bp, "GenericCarmCT")
        .def(py::init<DetectorBinning>(), "binning"_a = DetectorBinning::Binning1x1);
}
