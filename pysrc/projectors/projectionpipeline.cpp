#include <pybind11/pybind11.h>
#include <projectors/projectionpipeline.h>

namespace py = pybind11;

void init_projectionpipeline(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<ProjectionPipeline, AbstractProjector>(m, "ProjectionPipeline")
        .def(py::init<AbstractProjector*>(), "projector"_a = nullptr, py::keep_alive<1,2>())
        .def("project_composite", &ProjectionPipeline::projectComposite, "volume"_a)
        .def("is_linear", &ProjectionPipeline::isLinear)
        .def("append_extension", static_cast<void(ProjectionPipeline::*)(ProjectorExtension*)>
            (&ProjectionPipeline::appendExtension), "extension"_a, py::keep_alive<1,2>())
        .def("insert_extension", static_cast<void(ProjectionPipeline::*)(uint, ProjectorExtension*)>
            (&ProjectionPipeline::insertExtension), "pos"_a, "extension"_a, py::keep_alive<1,2>())
        .def("release_extension", &ProjectionPipeline::releaseExtension, "pos"_a)
        .def("remove_extension", &ProjectionPipeline::removeExtension, "pos"_a)
        .def("set_projector", static_cast<void(ProjectionPipeline::*)(AbstractProjector*)>
            (&ProjectionPipeline::setProjector), "projector"_a, py::keep_alive<1,2>())
        .def("extension", &ProjectionPipeline::extension, "pos"_a, rvp::reference)
        .def("projector", &ProjectionPipeline::projector, rvp::reference)
        .def("nb_extensions", &ProjectionPipeline::nbExtensions);
}
