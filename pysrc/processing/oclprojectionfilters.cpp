#include "pysrc/pybind_common.h"
#include <processing/oclprojectionfilters.h>
#include <acquisition/acquisitionsetup.h>
#include <acquisition/viewgeometry.h>
#include <img/projectiondata.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_oclprojectionfilters(py::module& m)
{
    using namespace CTL;
    using namespace OCL;
    using namespace py::literals;

    py::class_<CosineWeighting, GenericOCLProjectionFilter, PySharedPtr<CosineWeighting>>(m, "CosineWeighting")
        .def(py::init<>())
        .def("filter", static_cast<void(CosineWeighting::*)
            (ProjectionData&,const FullGeometry&)>(&CosineWeighting::filter),
            "projections"_a, "pmats"_a)
        .def("filter", static_cast<void(CosineWeighting::*)
            (ProjectionData&,const AcquisitionSetup&)>(&CosineWeighting::filter),
            "projections"_a, "setup"_a);

    py::class_<ParkerWeightingRev, GenericOCLProjectionFilter, PySharedPtr<ParkerWeightingRev>>(m, "ParkerWeightingRev")
        .def(py::init<float>(), "q"_a = 1.0f)
        .def("filter", static_cast<void(ParkerWeightingRev::*)
            (ProjectionData&,const FullGeometry&)>(&ParkerWeightingRev::filter),
            "projections"_a, "pmats"_a)
        .def("filter", static_cast<void(ParkerWeightingRev::*)
            (ProjectionData&,const AcquisitionSetup&)>(&ParkerWeightingRev::filter),
            "projections"_a, "setup"_a)
        .def("last_angular_range", &ParkerWeightingRev::lastAngularRange);

    auto af = py::class_<ApodizationFilter, GenericOCLProjectionFilter, PySharedPtr<ApodizationFilter>>(m, "ApodizationFilter");

    py::enum_<ApodizationFilter::FilterType>(af, "FilterType")
        .value("RamLak", ApodizationFilter::RamLak)
        .value("SheppLogan", ApodizationFilter::SheppLogan)
        .value("Cosine", ApodizationFilter::Cosine)
        .value("Hann", ApodizationFilter::Hann)
        .export_values();

    af.def(py::init<ApodizationFilter::FilterType, float>(), "filter_type"_a,
        "frequency_scaling"_a = 1.0f);
}
