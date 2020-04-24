#include <pybind11/pybind11.h>
#include <acquisition/ctsystembuilder.h>
#include <acquisition/systemblueprints.h>
#include <QString>

namespace py = pybind11;

void init_ctsystembuilder(py::module& m)
{
    using namespace CTL;
    using rvp = py::return_value_policy;
    
    py::class_<CTSystemBuilder>(m, "CTSystemBuilder")
        .def_static("create_from_blueprint", &CTSystemBuilder::createFromBlueprint)
        .def_static("create_from_json_file", [](const std::string& file_name)
        {
            return CTSystemBuilder::createFromJSONFile(QString::fromStdString(file_name));
        });

    py::class_<AbstractCTSystemBlueprint>(m, "AbstractCTSystemBlueprint")
        .def("system_name", [](const AbstractCTSystemBlueprint& self)
        {
            return self.systemName().toStdString();
        })
        .def("modifiers", &AbstractCTSystemBlueprint::modifiers);
}