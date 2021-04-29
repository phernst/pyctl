#include "pysrc/pybind_common.h"
#include <gui/widgets/acquisitionsetupview.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(CTL::AcquisitionSetup setup, uint maxNbViews, bool sourceOnly,
                     float visualScale)
{
    const auto nbViews = setup.nbViews();

    auto viewer = new CTL::gui::AcquisitionSetupView(nullptr, visualScale);
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setAcquisitionSetup(std::move(setup));
    viewer->setSourceOnly(sourceOnly);

    auto reqLeaveOut = static_cast<uint>(std::ceil(float(nbViews) / float(maxNbViews))) - 1u;
    viewer->showFullAcquisition(reqLeaveOut);

    FigureManager::instance().addFigure(viewer);
}

void init_acquisitionsetupview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::AcquisitionSetupView>(m, "AcquisitionSetupView")
        .def_static("plot", plot, "setup"_a, "max_nb_views"_a = 100, "source_only"_a = false,
            "visual_scale"_a = 50.f);

    m.def("plot", plot, "setup"_a, "max_nb_views"_a = 100, "source_only"_a = false,
        "visual_scale"_a = 50.0f);
}
