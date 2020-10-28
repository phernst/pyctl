#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <img/projectiondataview.h>

namespace py = pybind11;

void init_projectiondataview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ProjectionDataView>(m, "ProjectionDataView")
        .def(py::init<const ProjectionData&,const std::vector<uint>&>(),
            "projections"_a, "view_ids"_a)
        .def(py::init<const ProjectionData&>(), "projections"_a)
        .def("first", &ProjectionDataView::first)
        .def("view", &ProjectionDataView::view, "i"_a)
        .def("dimensions", &ProjectionDataView::dimensions)
        .def("view_dimensions", &ProjectionDataView::viewDimensions)
        .def("combined", &ProjectionDataView::combined, "layout"_a=ModuleLayout())
        .def("min", &ProjectionDataView::min)
        .def("max", &ProjectionDataView::max)
        .def("nb_views", &ProjectionDataView::nbViews)
        .def("to_vector", &ProjectionDataView::toVector)
        .def("contains_view", &ProjectionDataView::containsView, "view_id"_a)
        .def("shuffle", static_cast<void(ProjectionDataView::*)()>(&ProjectionDataView::shuffle))
        .def("shuffle", static_cast<void(ProjectionDataView::*)(uint)>(&ProjectionDataView::shuffle),
            "seed"_a)
        .def("shuffled", static_cast<ProjectionDataView(ProjectionDataView::*)()const>
            (&ProjectionDataView::shuffled))
        .def("shuffled", static_cast<ProjectionDataView(ProjectionDataView::*)(uint)const>
            (&ProjectionDataView::shuffled), "seed"_a)
        .def("subset", static_cast<ProjectionDataView(ProjectionDataView::*)
            (const std::vector<uint>&)const>(&ProjectionDataView::subset), "ids_in_view"_a)
        .def("subset", static_cast<ProjectionDataView(ProjectionDataView::*)
            (uint,uint)const>(&ProjectionDataView::subset), "pos"_a = 0,
            "count"_a = std::numeric_limits<uint>::max())
        .def("view_ids", &ProjectionDataView::viewIds);
}
