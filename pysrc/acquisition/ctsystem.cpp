#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <acquisition/ctsystem.h>
#include <components/abstractbeammodifier.h>
#include <components/abstractdetector.h>
#include <components/abstractgantry.h>
#include <components/abstractsource.h>
#include <algorithm>

namespace py = pybind11;

void init_ctsystem(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<CTsystem>(m, "CTsystem")
        .def(py::init<>())
        .def(py::init([](const std::string& name)
        {
            return CTsystem(QString::fromStdString(name));
        }), "name"_a = CTsystem::defaultName().toStdString())
        .def("info", [](const CTsystem& self)
        {
            return self.info().toStdString();
        })
        .def("overview", [](const CTsystem& self)
        {
            return self.overview().toStdString();
        })
        .def("detectors", &CTsystem::detectors, rvp::reference_internal)
        .def("gantries", &CTsystem::gantries, rvp::reference_internal)
        .def("sources", &CTsystem::sources, rvp::reference_internal)
        .def("modifiers", &CTsystem::modifiers, rvp::reference_internal)
        .def("components", [](const CTsystem& self)
        {
            const auto& comps { self.components() };
            auto raw_comps { std::vector<SystemComponent*>(comps.size()) };
            std::transform(comps.cbegin(), comps.cend(),
                raw_comps.begin(), [](const auto& comp) { return comp.get(); });
            return raw_comps;
        }, rvp::reference_internal)
        .def("name", [](const CTsystem& self)
        {
            return self.name().toStdString();
        })
        .def("nb_components", &CTsystem::nbComponents)
        .def("add_component", [](CTsystem& self, SystemComponent* c)
        {
            self.addComponent(c);
        }, "component"_a, py::keep_alive<0, 1>())
        .def("clear", &CTsystem::clear)
        .def("is_empty", &CTsystem::isEmpty)
        .def("is_valid", &CTsystem::isValid)
        .def("is_simple", &CTsystem::isSimple)
        .def("rename", [](CTsystem& self, const std::string& name)
        {
            self.rename(QString::fromStdString(name));
        }, "name"_a)
        .def("remove_component", &CTsystem::removeComponent)
        .def_static("default_name", []()
        {
            return CTsystem::defaultName().toStdString();
        });
}
