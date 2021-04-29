#include "pysrc/pybind_common.h"
#include <gui/widgets/volumeviewer.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static CTL::gui::VolumeViewer* plot(CTL::CompositeVolume data)
{
    auto viewer = new CTL::gui::VolumeViewer(std::move(data));
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->autoResize();
    viewer->setAutoMouseWindowScaling();
    FigureManager::instance().addFigure(viewer);
    return viewer;
}

void init_volumeviewer(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::VolumeViewer>(m, "VolumeViewer")
        .def_static("plot", [](CTL::CompositeVolume data)
        {
            plot(std::move(data));
        }, "data"_a)
        .def_static("plot", [](SpectralVolumeData data)
        {
            plot(CompositeVolume{ std::move(data) });
        }, "data"_a)
        .def_static("plot", [](VoxelVolume<float> data)
        {
            plot(CompositeVolume{ std::move(data) });
        }, "data"_a);
    
    m.def("plot", [](CTL::CompositeVolume data)
        {
            plot(std::move(data));
        }, "data"_a)
        .def("plot", [](SpectralVolumeData data)
        {
            plot(CompositeVolume{ std::move(data) });
        }, "data"_a)
        .def("plot", [](VoxelVolume<float> data)
        {
            plot(CompositeVolume{ std::move(data) });
        }, "data"_a);
}
