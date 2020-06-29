#include <pybind11/pybind11.h>
#include <acquisition/geometrydecoder.h>

namespace py = pybind11;

void init_geometrydecoder(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    using PhysicalDimension = GeometryDecoder::PhysicalDimension;
    
    auto gd { py::class_<GeometryDecoder>(m, "GeometryDecoder") };
    auto pd { py::enum_<PhysicalDimension>(gd, "PhysicalDimension") };
        
    // TODO - pybind11: Don't allow mixed static/non-static overloads #804
    gd.def_static("decode_single_view_geometry", [](const SingleViewGeometry& singleViewGeometry,
                                                      py::tuple pixelPerModule,
                                                      PhysicalDimension physicalDimension,
                                                      double mm)
        {
            return GeometryDecoder::decodeSingleViewGeometry(
                singleViewGeometry,
                { pixelPerModule[0].cast<int>(), pixelPerModule[1].cast<int>() },
                physicalDimension,
                mm);
        }, "single_view_geometry"_a, "pixel_per_module"_a, "physical_dimension"_a =
            PhysicalDimension::PixelWidth, "mm"_a = 1.0)
        .def_static("decode_full_geometry", [](const FullGeometry& geometry,
                                               py::tuple pixelPerModule,
                                               py::tuple pixelDimensions)
        {
            return GeometryDecoder::decodeFullGeometry(
                geometry,
                { pixelPerModule[0].cast<int>(), pixelPerModule[1].cast<int>() },
                { pixelPerModule[0].cast<qreal>(), pixelPerModule[1].cast<qreal>() });
        }, "geometry"_a, "pixel_per_module"_a, "pixel_dimensions"_a)
        .def_static("decode_full_geometry", [](const FullGeometry& geometry,
                                               py::tuple pixelPerModule,
                                               PhysicalDimension physicalDimension,
                                               double mm)
        {
            return GeometryDecoder::decodeFullGeometry(
                geometry,
                { pixelPerModule[0].cast<int>(), pixelPerModule[1].cast<int>() },
                physicalDimension,
                mm);
        }, "geometry"_a, "pixel_per_module"_a, "physical_dimension"_a =
            PhysicalDimension::PixelWidth, "mm"_a = 1.0);

    pd.value("PixelWidth", PhysicalDimension::PixelWidth)
        .value("PixelHeight", PhysicalDimension::PixelHeight)
        .value("SourceDetectorDistance", PhysicalDimension::SourceDetectorDistance)
        .export_values();
}
