#include <pybind11/pybind11.h>
#include <components/xraytube.h>

namespace py = pybind11;

void init_xraytube(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<XrayTube, AbstractSource, std::shared_ptr<XrayTube>>(m, "XrayTube")
        .def("set_tube_voltage", &XrayTube::setTubeVoltage, "voltage"_a)
        .def("set_milliampere_seconds", &XrayTube::setMilliampereSeconds, "mas"_a);
}
