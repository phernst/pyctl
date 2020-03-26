#include <pybind11/pybind11.h>
#include <projectors/arealfocalspotextension.h>

namespace py = pybind11;

void init_arealfocalspotextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<ArealFocalSpotExtension, ProjectorExtension>(m, "ArealFocalSpotExtension")
        .def(py::init<>())
        .def(py::init([](py::tuple discretization, bool lowExtAppr)
        {
            return std::make_unique<ArealFocalSpotExtension>(
                QSize(discretization[0].cast<int>(), discretization[1].cast<int>()),
                lowExtAppr);
        }), "discretization"_a, "low_extinction_approximation"_a = false)
        .def("set_discretization", [](ArealFocalSpotExtension* self, py::tuple discr)
        {
            self->setDiscretization({ discr[0].cast<int>(), discr[1].cast<int>() });
        }, "discretization"_a)
        .def("enable_low_extinction_approximization",
            &ArealFocalSpotExtension::enableLowExtinctionApproximation, "enable"_a = true)
        .def("is_linear", &ArealFocalSpotExtension::isLinear)
        .def("configure", &ArealFocalSpotExtension::configure, "setup"_a, py::keep_alive<1,2>());
}
