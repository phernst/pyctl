#include <pybind11/pybind11.h>
#include <gui/widgets/datamodelviewer.h>
#include <models/abstractdatamodel.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

static void plot(std::shared_ptr<CTL::AbstractDataModel> model,
                 const std::string& labelX,
                 const std::string& labelY)
{
    auto viewer = new CTL::gui::DataModelViewer;
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setData(std::move(model));

    viewer->setLabelX(QString::fromStdString(labelX));
    viewer->setLabelY(QString::fromStdString(labelY));

    FigureManager::instance().addFigure(viewer);
}

void init_datamodelviewer(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::DataModelViewer>(m, "DataModelViewer")
        .def_static("plot", plot, "model"_a, "label_x"_a = "x", "label_y"_a = "y");
    
    m.def("plot", plot, "model"_a, "label_x"_a = "x", "label_y"_a = "y");
}
