#include <pybind11/pybind11.h>
#include <components/abstractsource.h>

namespace py = pybind11;

void init_abstractsource(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<AbstractSource, SystemComponent, std::shared_ptr<AbstractSource>>(m, "AbstractSource");
}
