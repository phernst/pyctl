#include <pybind11/pybind11.h>
#include <gui/widgets/volumesliceviewer.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::VoxelVolume<float> volume)
{
    const auto maxDim = std::max(std::max(volume.dimensions().x, volume.dimensions().y),
                                 volume.dimensions().z);

    auto viewer = new CTL::gui::VolumeSliceViewer;
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setData(volume);

    FigureManager::instance().addFigure(viewer);
}

void init_volumesliceviewer(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::VolumeSliceViewer>(m, "VolumeSliceViewer")
        .def_static("plot", plot, "volume"_a);
    
    m.def("plot", plot, "volume"_a);
}
