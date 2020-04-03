#include <pybind11/pybind11.h>
#include <gui/widgets/ctsystemview.h>
#include <acquisition/simplectsystem.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::SimpleCTsystem system, float visualScale)
{
    auto viewer = new CTL::gui::CTSystemView(nullptr, visualScale);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setCTSystem(std::move(system));

    FigureManager::instance().addFigure(viewer);
}

void init_ctsystemview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::CTSystemView>(m, "CTSystemView")
        .def_static("plot", plot, "system"_a, "visual_scale"_a = 50.f);
}
