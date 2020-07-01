#include <pybind11/pybind11.h>
#include <components/systemcomponent.h>

namespace py = pybind11;

void init_systemcomponent(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    // TODO - use PySharedPtr
    py::class_<SystemComponent, std::shared_ptr<SystemComponent>>(m, "SystemComponent")
        .def(py::init<>())
        .def("elemental_type", &SystemComponent::elementalType)
        .def("info", [](const SystemComponent& self)
        {
            return self.info().toStdString();
        })
        .def("name", [](const SystemComponent& self)
        {
            return self.name().toStdString();
        })
        .def("rename", [](SystemComponent& self, const std::string& name)
        {
            self.rename(QString::fromStdString(name));
        }, "name"_a);
}
