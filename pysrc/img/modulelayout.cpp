#include <pybind11/pybind11.h>
#include <img/modulelayout.h>

namespace py = pybind11;

void init_modulelayout(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ModuleLayout>(m, "ModuleLayout")
        .def(py::init<uint,uint>(), "nb_rows"_a = 0, "nb_cols"_a = 0)
        .def("__getitem__", [](const ModuleLayout& self, py::tuple t)
        {
            return self(t[0].cast<uint>(), t[1].cast<uint>());
        }, "position"_a, "Returns the module index at position (row, col)")
        .def("columns", &ModuleLayout::columns)
        .def("rows", &ModuleLayout::rows)
        .def("is_empty", &ModuleLayout::isEmpty)
        .def_static("canonic_layout", &ModuleLayout::canonicLayout, "nb_rows"_a,
            "nb_cols"_a, "row_major_order"_a = true);
        
}
