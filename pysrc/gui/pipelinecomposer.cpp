#include "pysrc/pybind_common.h"
#include <gui/widgets/pipelinecomposer.h>
#include <projectors/projectionpipeline.h>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

void init_pipelinecomposer(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<gui::PipelineComposer>(m, "PipelineComposer")
        .def_static("from_dialog", gui::PipelineComposer::fromDialog);
}
