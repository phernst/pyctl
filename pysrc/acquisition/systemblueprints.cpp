#include <pybind11/pybind11.h>
#include <acquisition/systemblueprints.h>

namespace py = pybind11;

void init_systemblueprints(py::module& m, py::module& bp)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::enum_<DetectorBinning>(m, "DetectorBinning")
        .value("Binning1x1", DetectorBinning::Binning1x1)
        .value("Binning2x2", DetectorBinning::Binning2x2)
        .value("Binning4x4", DetectorBinning::Binning4x4);

    py::class_<blueprints::GenericTubularCT, AbstractCTSystemBlueprint>(bp, "GenericTubularCT")
        .def(py::init<>())
        .def("detector", &blueprints::GenericTubularCT::detector, rvp::reference)
        .def("gantry", &blueprints::GenericTubularCT::gantry, rvp::reference)
        .def("source", &blueprints::GenericTubularCT::source, rvp::reference)
        .def("system_name", [](const blueprints::GenericTubularCT& self)
        {
            return self.systemName().toStdString();
        });
    py::class_<blueprints::GenericCarmCT, AbstractCTSystemBlueprint>(bp, "GenericCarmCT")
        .def(py::init<DetectorBinning>(), "binning"_a = DetectorBinning::Binning2x2)
        .def("detector", &blueprints::GenericCarmCT::detector, rvp::reference)
        .def("gantry", &blueprints::GenericCarmCT::gantry, rvp::reference)
        .def("source", &blueprints::GenericCarmCT::source, rvp::reference)
        .def("system_name", [](const blueprints::GenericCarmCT& self)
        {
            return self.systemName().toStdString();
        });
}
