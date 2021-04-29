#include "pysrc/pybind_common.h"
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

    py::class_<CTSystem>(m, "CTSystem")
        .def(py::init<>())
        .def(py::init([](const std::string& name)
        {
            return CTSystem(QString::fromStdString(name));
        }), "name"_a = CTSystem::defaultName().toStdString())
        .def("info", [](const CTSystem& self)
        {
            return self.info().toStdString();
        })
        .def("overview", [](const CTSystem& self)
        {
            return self.overview().toStdString();
        })
        .def("detectors", &CTSystem::detectors, rvp::reference_internal)
        .def("gantries", &CTSystem::gantries, rvp::reference_internal)
        .def("sources", &CTSystem::sources, rvp::reference_internal)
        .def("modifiers", &CTSystem::modifiers, rvp::reference_internal)
        .def("components", [](const CTSystem& self)
        {
            const auto& comps { self.components() };
            auto raw_comps { std::vector<SystemComponent*>(comps.size()) };
            std::transform(comps.cbegin(), comps.cend(),
                raw_comps.begin(), [](const CTsystem::ComponentPtr& comp) { return comp.get(); });
            return raw_comps;
        }, rvp::reference_internal)
        .def("name", [](const CTSystem& self)
        {
            return self.name().toStdString();
        })
        .def("nb_components", &CTSystem::nbComponents)
        .def("add_component", [](CTSystem& self, SystemComponent* c)
        {
            self.addComponent(c->clone());  // TODO: modify when PySharedPtr is used
        }, "component"_a, py::keep_alive<1, 2>())
        .def("clear", &CTSystem::clear)
        .def("is_empty", &CTSystem::isEmpty)
        .def("is_valid", &CTSystem::isValid)
        .def("is_simple", &CTSystem::isSimple)
        .def("rename", [](CTSystem& self, const std::string& name)
        {
            self.rename(QString::fromStdString(name));
        }, "name"_a)
        .def("remove_component", &CTSystem::removeComponent)
        .def_static("default_name", []()
        {
            return CTSystem::defaultName().toStdString();
        });
}
