#include "pysrc/pybind_common.h"
#include <components/abstractsource.h>

namespace py = pybind11;

void init_abstractsource(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<AbstractSource, SystemComponent, std::shared_ptr<AbstractSource>>(m, "AbstractSource")
        .def("info", [](const AbstractSource& self)
        {
            return self.info().toStdString();
        })
        .def("energy_range", &AbstractSource::energyRange)
        .def("mean_energy", &AbstractSource::meanEnergy)
        .def("photon_flux", &AbstractSource::photonFlux)
        .def("flux_modifier", &AbstractSource::fluxModifier)
        .def("focal_spot_size", [](const AbstractSource& self)
        {
            auto pysize { py::tuple(2) };
            auto ctlsize { self.focalSpotSize() };
            pysize[0] = ctlsize.width();
            pysize[1] = ctlsize.height();
            return pysize;
        })
        .def("focal_spot_position", &AbstractSource::focalSpotPosition)
        .def("set_flux_modifier", &AbstractSource::setFluxModifier, "modifier"_a)
        .def("set_focal_spot_size", [](AbstractSource& self, py::tuple pysize)
        {
            if (pysize.size() != 2)
            {
                throw std::invalid_argument("Expected length of tuple is 2");
            }
            self.setFocalSpotSize({ pysize[0].cast<double>(), pysize[1].cast<double>() });
        }, "size"_a)
        .def("set_focal_spot_size", [](AbstractSource& self, py::list pysize)
        {
            if (pysize.size() != 2)
            {
                throw std::invalid_argument("Expected length of list is 2");
            }
            self.setFocalSpotSize({ pysize[0].cast<double>(), pysize[1].cast<double>() });
        }, "size"_a)
        .def("set_focal_spot_size", static_cast<void(AbstractSource::*)(double,double)>
            (&AbstractSource::setFocalSpotSize), "width"_a, "height"_a)
        .def("set_focal_spot_position", static_cast<void(AbstractSource::*)(const Vector3x1&)>
            (&AbstractSource::setFocalSpotPosition), "position"_a)
        .def("set_focal_spot_position", [](AbstractSource& self, py::tuple pypos)
        {
            if (pypos.size() != 3)
            {
                throw std::invalid_argument("Expected length of tuple is 3");
            }
            self.setFocalSpotPosition(
                pypos[0].cast<double>(),
                pypos[1].cast<double>(),
                pypos[2].cast<double>());
        }, "position"_a)
        .def("set_focal_spot_position", [](AbstractSource& self, py::list pypos)
        {
            if (pypos.size() != 3)
            {
                throw std::invalid_argument("Expected length of list is 3");
            }
            self.setFocalSpotPosition(
                pypos[0].cast<double>(),
                pypos[1].cast<double>(),
                pypos[2].cast<double>());
        }, "position"_a)
        .def("set_focal_spot_position", static_cast<void(AbstractSource::*)(double,double,double)>
            (&AbstractSource::setFocalSpotPosition), "x"_a, "y"_a, "z"_a)
        .def("set_energy_range_restriction", &AbstractSource::setEnergyRangeRestriction,
            "window"_a);
}
