#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <img/projectiondataview.h>

namespace py = pybind11;

void init_projectiondataview(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using PDV_ = ProjectionDataView;

    py::class_<ProjectionDataView>(m, "ProjectionDataView")
        .def(py::init<const ProjectionData&,std::vector<uint>>(), "projections"_a, "view_ids"_a)
        .def(py::init<const ProjectionData&>(), "projections"_a)
        .def(py::init<>())
        .def("reset_data", static_cast<void(ProjectionDataView::*)(const ProjectionData&,
            std::vector<uint>)>(&ProjectionDataView::resetData), "projections"_a, "view_ids"_a)
        .def("first", &ProjectionDataView::first)
        .def("view", &ProjectionDataView::view, "i"_a)
        .def("dimensions", &ProjectionDataView::dimensions)
        .def("view_dimensions", &ProjectionDataView::viewDimensions)
        .def("combined", &ProjectionDataView::combined, "layout"_a = ModuleLayout{})
        .def("is_valid", &ProjectionDataView::isValid)
        .def("max", &ProjectionDataView::max)
        .def("min", &ProjectionDataView::min)
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
        .def("subset", static_cast<ProjectionDataView(ProjectionDataView::*)(const std::vector<uint>&)const>
            (&ProjectionDataView::subset), "ids_in_view"_a)
        .def("subset", static_cast<ProjectionDataView(ProjectionDataView::*)(uint,uint)const>
            (&ProjectionDataView::subset), "pos"_a = 0, "count"_a = std::numeric_limits<uint>::max())
        .def("view_ids", &ProjectionDataView::viewIds)
        .def("data_copy", &ProjectionDataView::dataCopy)
        .def("__eq__", &ProjectionDataView::operator==, "other"_a)
        .def("__ne__", &ProjectionDataView::operator!=, "other"_a)
        .def("__add__", &ProjectionDataView::operator+, "other"_a)
        .def("__sub__", &ProjectionDataView::operator-, "other"_a)
        .def("__mul__", &ProjectionDataView::operator*, "factor"_a)
        .def("__truediv__", &ProjectionDataView::operator/, "divisor"_a);

    py::implicitly_convertible<ProjectionData, ProjectionDataView>();
}
