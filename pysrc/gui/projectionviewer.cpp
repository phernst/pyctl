#include "pysrc/pybind_common.h"
#include <gui/widgets/projectionviewer.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::ProjectionData projections)
{
    auto viewer = new CTL::gui::ProjectionViewer(projections);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setModuleLayout({});
    viewer->autoResize();
    viewer->setAutoMouseWindowScaling();

    FigureManager::instance().addFigure(viewer);
}

void init_projectionviewer(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::ProjectionViewer>(m, "ProjectionViewer")
        .def_static("plot", plot, "projections"_a);
    
    m.def("plot", plot, "projections"_a);
}
