#include <pybind11/pybind11.h>
#include <acquisition/acquisitionsetup.h>

namespace py = pybind11;

void init_acquisitionsetup(py::module& m)
{
    using namespace CTL;
    py::class_<AcquisitionSetup>(m, "AcquisitionSetup")
        .def(py::init<>())
        .def(py::init<const CTsystem&, uint>())
        .def("apply_preparation_protocol", &AcquisitionSetup::applyPreparationProtocol)
        .def("is_valid", &AcquisitionSetup::isValid);
}