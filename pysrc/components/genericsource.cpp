#include <pybind11/pybind11.h>
#include <components/genericsource.h>
#include <acquisition/simplectsystem.h>
#include "pysrc/utils.h"

namespace py = pybind11;

void init_genericsource(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<GenericSource, AbstractSource, std::shared_ptr<GenericSource>>(m, "GenericSource")
        .def(py::init([](py::tuple focalSpotSize,
                         const Vector3x1& focalSpotPosition,
                         const std::string& name) -> GenericSource
        {
            if (focalSpotSize.size() != 2)
            {
                throw std::invalid_argument("focal_spot_size must be a tuple of 2 elements");
            }
            return {
                { focalSpotSize[0].cast<double>(), focalSpotSize[1].cast<double>() },
                focalSpotPosition,
                QString::fromStdString(name)
            };
        }), "focal_spot_size"_a = zero_tuple<double,2>(), "focal_spot_position"_a = Vector3x1(0.0),
            "name"_a = GenericSource::defaultName().toStdString())
        .def("info", [](const GenericSource& self) { return self.info().toStdString(); })
        .def("spectrum_discretization_hint", &GenericSource::spectrumDiscretizationHint)
        .def("set_energy_range", &GenericSource::setEnergyRange, "range"_a)
        .def_static("default_name", [](){ return GenericSource::defaultName().toStdString(); })
        .def_static("set_photon_count_in_system", &GenericSource::setPhotonCountInSystem,
            "system"_a, "photons_per_pixel"_a);
}
