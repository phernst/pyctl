#include "pysrc/pybind_common.h"
#include <components/abstractdetector.h>

namespace py = pybind11;

void init_abstractdetector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    auto ad = py::class_<AbstractDetector, SystemComponent, std::shared_ptr<AbstractDetector>>(m, "AbstractDetector")
        .def("info", [](const AbstractDetector* self)
        {
            return self->info().toStdString();
        })
        .def("set_saturation_model", static_cast<void(AbstractDetector::*)
            (AbstractDataModel*,AbstractDetector::SaturationModelType)>
            (&AbstractDetector::setSaturationModel), "model"_a, "type"_a, py::keep_alive<1,2>())
        .def("set_spectral_response_model", static_cast<void(AbstractDetector::*)
            (AbstractDataModel*)>(&AbstractDetector::setSpectralResponseModel), "model"_a,
            py::keep_alive<1,2>())
        .def("nb_detector_modules", &AbstractDetector::nbDetectorModules)
        .def("nb_pixel_per_module", [](const AbstractDetector* self)
        {
            auto ppm { self->nbPixelPerModule() };
            auto pyppm { py::tuple(2) };
            pyppm[0] = ppm.width();
            pyppm[1] = ppm.height();
            return pyppm;
        })
        .def("pixel_dimensions", [](const AbstractDetector* self)
        {
            auto pdim { self->pixelDimensions() };
            auto pypdim { py::tuple(2) };
            pypdim[0] = pdim.width();
            pypdim[1] = pdim.height();
            return pypdim;
        })
        .def("module_location", &AbstractDetector::moduleLocation, "module"_a)
        .def("saturation_model", &AbstractDetector::saturationModel, rvp::reference)
        .def("spectral_response_model", &AbstractDetector::spectralResponseModel, rvp::reference)
        .def("saturation_model_type", &AbstractDetector::saturationModelType)
        .def("skew_angle", &AbstractDetector::skewAngle)
        .def("has_saturation_model", &AbstractDetector::hasSaturationModel)
        .def("has_spectral_response_model", &AbstractDetector::hasSpectralResponseModel)
        .def("module_dimensions", [](const AbstractDetector* self)
        {
            auto mdim { self->moduleDimensions() };
            auto pymdim { py::tuple(2) };
            pymdim[0] = mdim.width();
            pymdim[1] = mdim.height();
            return pymdim;
        })
        .def("view_dimensions", &AbstractDetector::viewDimensions);

    py::enum_<AbstractDetector::SaturationModelType>(ad, "SaturationModelType")
        .value("Extinction", AbstractDetector::SaturationModelType::Extinction)
        .value("PhotonCount", AbstractDetector::SaturationModelType::PhotonCount)
        .value("Intensity", AbstractDetector::SaturationModelType::Intensity)
        .value("Undefined", AbstractDetector::SaturationModelType::Undefined)
        .export_values();
}
