#include "pysrc/pybind_common.h"
#include <components/abstractbeammodifier.h>

namespace py = pybind11;

void init_abstractbeammodifier(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<AbstractBeamModifier, SystemComponent, std::shared_ptr<AbstractBeamModifier>>(m, "AbstractBeamModifier")
        .def("info", [](const AbstractBeamModifier& self)
        {
            return self.info().toStdString();
        });
}
