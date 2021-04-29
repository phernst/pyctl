#include "pysrc/pybind_common.h"
#include <components/xraytube.h>
#include "pysrc/utils.h"

namespace py = pybind11;

void init_xraytube(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<XrayTube, AbstractSource, std::shared_ptr<XrayTube>>(m, "XrayTube")
        .def(py::init([](py::tuple focalSpotSize,
                         const Vector3x1& focalSpotPosition,
                         double tubeVoltage,
                         double mAs,
                         const std::string& name) -> XrayTube
        {
            if (focalSpotSize.size() != 2)
            {
                throw std::invalid_argument("focal_spot_size must be a tuple of 2 elements");
            }
            return {
                { focalSpotSize[0].cast<double>(), focalSpotSize[1].cast<double>() },
                focalSpotPosition,
                tubeVoltage,
                mAs,
                QString::fromStdString(name)
            };
        }), "focal_spot_size"_a = zero_tuple<double, 2>(), "focal_spot_position"_a = Vector3x1(0.0),
            "tube_voltage"_a = 100.0, "mas"_a = 1.0, "name"_a = XrayTube::defaultName().toStdString())
        .def("info", [](const XrayTube& self) { return self.info().toStdString(); })
        .def("spectrum_discretization_hint", &XrayTube::spectrumDiscretizationHint)
        .def("tube_voltage", &XrayTube::tubeVoltage)
        .def("mas", &XrayTube::mAs)
        .def("set_tube_voltage", &XrayTube::setTubeVoltage, "voltage"_a)
        .def("set_milliampere_seconds", &XrayTube::setMilliampereSeconds, "mas"_a)
        .def_static("default_name", [](){ return XrayTube::defaultName().toStdString(); });
}
