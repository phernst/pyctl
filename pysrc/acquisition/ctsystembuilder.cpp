#include <pybind11/pybind11.h>
#include <acquisition/ctsystembuilder.h>
#include <acquisition/systemblueprints.h>
#include <QString>

namespace py = pybind11;

static CTL::CTsystem create_from_json_file(const std::string& file_name)
{
    return CTL::CTsystemBuilder::createFromJSONFile(QString::fromStdString(file_name));
}

void init_ctsystembuilder(py::module& m)
{
    using namespace CTL;
    py::class_<CTsystemBuilder>(m, "CTsystemBuilder")
        .def_static("create_from_blueprint", &CTsystemBuilder::createFromBlueprint)
        .def_static("create_from_json_file", &create_from_json_file);

    py::class_<AbstractCTsystemBlueprint>(m, "AbstractCTsystemBlueprint")
        .def("detector", &AbstractCTsystemBlueprint::detector)
        .def("gantry", &AbstractCTsystemBlueprint::gantry)
        .def("source", &AbstractCTsystemBlueprint::source);

    
    auto bp { m.def_submodule("blueprints") };
    py::class_<blueprints::GenericTubularCT, AbstractCTsystemBlueprint>(bp, "GenericTubularCT")
        .def(py::init<>());
    py::class_<blueprints::GenericCarmCT, AbstractCTsystemBlueprint>(bp, "GenericCarmCT")
        .def(py::init<>());
}