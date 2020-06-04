#include <pybind11/pybind11.h>
#include <gui/widgets/chunk2dview.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::Chunk2D<float> data, py::tuple windowing, double zoom)
{
    auto viewer = new CTL::gui::Chunk2DView;

    viewer->setWindowing(windowing[0].cast<double>(), windowing[1].cast<double>());
    viewer->setZoom(zoom);

    viewer->setData(std::move(data));
    viewer->autoResize();
    viewer->setAutoMouseWindowScaling();
    viewer->setAttribute(Qt::WA_DeleteOnClose);

    FigureManager::instance().addFigure(viewer);
}

void init_chunk2dview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::Chunk2DView>(m, "Chunk2DView")
        .def_static("plot", plot, "data"_a, "windowing"_a = py::make_tuple(0.0, 0.0), "zoom"_a = 1.0);

    m.def("plot", plot, "data"_a, "windowing"_a = py::make_tuple(0.0, 0.0), "zoom"_a = 1.0);
}
