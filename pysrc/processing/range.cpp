#include <pybind11/pybind11.h>
#include <processing/range.h>
#include <sstream>

namespace py = pybind11;

template<typename T>
static py::class_<CTL::Range<T>> createPyRange(py::module& m, const char* name)
{
    using namespace CTL;
    using namespace py::literals;

    return py::class_<Range<T>>(m, name)
        .def(py::init<T, T>(), "start"_a, "end"_a)
        .def(py::init([](py::tuple pyrange) -> Range<T>
        {
            if (pyrange.size() != 2)
            {
                throw std::invalid_argument("Expected length of tuple is 2");
            }
            return { pyrange[0].cast<T>(), pyrange[1].cast<T>() };
        }), "range"_a)
        .def("start", static_cast<const T&(Range<T>::*)()const>(&Range<T>::start))
        .def("end", static_cast<const T&(Range<T>::*)()const>(&Range<T>::end))
        .def("width", &Range<T>::width)
        .def("spacing", &Range<T>::spacing, "nb_samples"_a)
        .def("center", &Range<T>::center)
        .def("linspace", static_cast<std::vector<T>(Range<T>::*)(uint)const>(&Range<T>::linspace),
            "nb_samples"_a)
        .def("__repr__", [](const Range<T>& self)
        {
            auto repr { std::stringstream() };
            repr << "Range(start=" << self.start() << ", end=" << self.end();
            repr << ", dtype=" << py::format_descriptor<T>::format() << ")";
            return repr.str();
        });
}

void init_range(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    createPyRange<float>(m, "RangeF");
    py::implicitly_convertible<py::tuple, Range<float>>();
}
