#include <pybind11/pybind11.h>
#include <components/genericbeammodifier.h>

namespace py = pybind11;

void init_genericbeammodifier(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<GenericBeamModifier, AbstractBeamModifier, std::shared_ptr<GenericBeamModifier>>(m, "GenericBeamModifier")
        .def(py::init([](const std::string& name) -> GenericBeamModifier
        {
            return { QString::fromStdString(name) };
        }), "name"_a = GenericBeamModifier::defaultName().toStdString())
        .def("info", [](const GenericBeamModifier& self) { return self.info().toStdString(); })
        .def_static("default_name", [](){ return GenericBeamModifier::defaultName().toStdString(); });
}
