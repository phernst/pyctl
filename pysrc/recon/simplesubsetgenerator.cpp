#include "pysrc/pybind_common.h"
#include <recon/simplesubsetgenerator.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_simplesubsetgenerator(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto ssg = py::class_<SimpleSubsetGenerator, AbstractFixedSizeSubsetGenerator, PySharedPtr<SimpleSubsetGenerator>>(m, "SimpleSubsetGenerator");

    py::enum_<SimpleSubsetGenerator::Order>(ssg, "Order")
        .value("Random", SimpleSubsetGenerator::Random)
        .value("Adjacent", SimpleSubsetGenerator::Adjacent)
        .value("Orthogonal180", SimpleSubsetGenerator::Orthogonal180)
        .value("Orthogonal360", SimpleSubsetGenerator::Orthogonal360)
        .export_values();

    ssg.def(py::init<uint, SimpleSubsetGenerator::Order>(),
            "nb_subsets"_a = 1, "subset_order"_a = SimpleSubsetGenerator::Random)
        .def("set_order", &SimpleSubsetGenerator::setOrder, "order"_a);
}
