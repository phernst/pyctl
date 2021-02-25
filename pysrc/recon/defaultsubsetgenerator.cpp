#include <pybind11/pybind11.h>
#include <recon/defaultsubsetgenerator.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_defaultsubsetgenerator(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto dsg = py::class_<DefaultSubsetGenerator, TransitionSchemeExtension, PySharedPtr<DefaultSubsetGenerator>>(m, "DefaultSubsetGenerator");

    py::enum_<DefaultSubsetGenerator::Order>(dsg, "Order")
        .value("Random", DefaultSubsetGenerator::Random)
        .value("Adjacent", DefaultSubsetGenerator::Adjacent)
        .value("Orthogonal180", DefaultSubsetGenerator::Orthogonal180)
        .value("Orthogonal360", DefaultSubsetGenerator::Orthogonal360)
        .value("RealOrthogonal", DefaultSubsetGenerator::RealOrthogonal)
        .export_values();

    dsg.def(py::init<>())
        .def("set_fixed_number_of_subsets", &DefaultSubsetGenerator::setFixedNumberOfSubsets,
            "nb_subsets"_a)
        .def("set_order", &DefaultSubsetGenerator::setOrder, "order"_a);
}
