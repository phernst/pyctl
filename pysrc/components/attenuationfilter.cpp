#include "pysrc/pybind_common.h"
#include <components/attenuationfilter.h>

namespace py = pybind11;

void init_attenuationfilter(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<AttenuationFilter, AbstractBeamModifier, std::shared_ptr<AttenuationFilter>>(m, "AttenuationFilter")
        .def(py::init<database::Composite,float>(), "material"_a, "mm"_a)
        .def(py::init<database::Element,float>(), "material"_a, "mm"_a)
        .def(py::init<std::shared_ptr<AbstractIntegrableDataModel>,float,float>(),
            "attenuation_model"_a, "mm"_a, "density"_a)
        .def("info", [](const AttenuationFilter& self) { return self.info().toStdString(); });
}
