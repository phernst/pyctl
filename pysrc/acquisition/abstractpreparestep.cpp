#include <pybind11/pybind11.h>
#include <acquisition/abstractpreparestep.h>
#include <acquisition/acquisitionsetup.h>

namespace py = pybind11;

void init_abstractpreparestep(py::module& m)
{
    using namespace CTL;
    py::class_<AbstractPreparationProtocol>(m, "AbstractPreparationProtocol")
        .def("is_applicable_to", &AbstractPreparationProtocol::isApplicableTo);
}