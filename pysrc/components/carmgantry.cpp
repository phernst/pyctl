#include "pysrc/pybind_common.h"
#include <components/carmgantry.h>

namespace py = pybind11;

void init_carmgantry(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<CarmGantry, AbstractGantry, std::shared_ptr<CarmGantry>>(m, "CarmGantry")
        .def(py::init([](double span, const std::string& name)
        {
            return CarmGantry(span, QString::fromStdString(name));
        }), "c_arm_span"_a = 1000.0, "name"_a = CarmGantry::defaultName().toStdString())
        .def("info", [](const CarmGantry& self)
        {
            return self.info().toStdString();
        })
        .def("location", &CarmGantry::location)
        .def("c_arm_span", &CarmGantry::cArmSpan)
        .def("set_location", &CarmGantry::setLocation, "location"_a)
        .def("set_c_arm_span", &CarmGantry::setCarmSpan, "span"_a)
        .def_static("default_name", [](){ return CarmGantry::defaultName().toStdString(); });
}
