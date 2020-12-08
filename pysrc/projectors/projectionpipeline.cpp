#include <pybind11/pybind11.h>
#include <projectors/projectionpipeline.h>
#include "pysrc/pysharedptr.h"

namespace py = pybind11;

void init_projectionpipeline(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    using ExtensionPtr = PySharedPtr<ProjectorExtension>;
    using ProjectorPtr = PySharedPtr<AbstractProjector>;

    py::class_<ProjectionPipeline, AbstractProjector, PySharedPtr<ProjectionPipeline>>(m, "ProjectionPipeline")
        .def(py::init([](const PySharedPtr<AbstractProjector>& proj)
        {
            return make_pysharedptr<ProjectionPipeline>(proj.release_to_ctl());
        }), "projector"_a = nullptr, py::keep_alive<1,2>())
        .def("project_composite", &ProjectionPipeline::projectComposite, "volume"_a)
        .def("project_sparse", &ProjectionPipeline::projectSparse, "volume"_a)
        .def("is_linear", &ProjectionPipeline::isLinear)
        .def("append_extension", [](ProjectionPipeline& self, const ExtensionPtr& ext)
        {
            self.appendExtension(ext.release_to_ctl());
        }, "extension"_a, py::keep_alive<1,2>())
        .def("insert_extension", [](ProjectionPipeline& self, uint pos, const ExtensionPtr& ext)
        {
            self.insertExtension(pos, ext.release_to_ctl());
        }, "pos"_a, "extension"_a, py::keep_alive<1,2>())
        .def("release_extension", &ProjectionPipeline::releaseExtension, "pos"_a)
        .def("remove_extension", &ProjectionPipeline::removeExtension, "pos"_a)
        .def("set_projector", [](ProjectionPipeline& self, const ProjectorPtr& proj)
        {
            self.setProjector(proj.release_to_ctl());
        }, "projector"_a, py::keep_alive<1,2>())
        .def("extension", &ProjectionPipeline::extension, "pos"_a, rvp::reference)
        .def("projector", &ProjectionPipeline::projector, rvp::reference)
        .def("nb_extensions", &ProjectionPipeline::nbExtensions);
}
