#include <pybind11/pybind11.h>
#include <gui/widgets/intersectionplaneview.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::VoxelVolume<float> volume,
    double azimuth, double polar, double distance, float visualScale)
{
    const auto maxDim = std::max(std::max(volume.dimensions().x, volume.dimensions().y),
                                 volume.dimensions().z);

    auto viewer = new CTL::gui::IntersectionPlaneView(nullptr, visualScale);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setVolumeDim(volume);
    viewer->setPlaneSize( { double(maxDim) * std::sqrt(3.0), double(maxDim) * std::sqrt(3.0) } );
    viewer->setPlaneParameter(azimuth, polar, distance);

    FigureManager::instance().addFigure(viewer);
}

void init_intersectionplaneview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::IntersectionPlaneView>(m, "IntersectionPlaneView")
        .def_static("plot", plot, "volume"_a, "azimuth"_a, "polar"_a, "distance"_a,
            "visual_scale"_a = 50.f);
}
