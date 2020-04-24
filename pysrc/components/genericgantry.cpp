#include <pybind11/pybind11.h>
#include <components/genericgantry.h>

namespace py = pybind11;

void init_genericgantry(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<GenericGantry, AbstractGantry, std::shared_ptr<GenericGantry>>(m, "GenericGantry")
        .def(py::init([](const std::string& name) -> GenericGantry
        {
            return { QString::fromStdString(name) };
        }), "name"_a = GenericGantry::defaultName().toStdString())
        .def(py::init([](const mat::Location& sourceLocation,
                         const mat::Location& detectorLocation,
                         const std::string& name) -> GenericGantry
        {
            return {
                sourceLocation,
                detectorLocation,
                QString::fromStdString(name)
            };
        }), "source_location"_a, "detector_location"_a,
            "name"_a = GenericGantry::defaultName().toStdString())
        .def(py::init([](const Vector3x1& sourcePosition,
                         const Matrix3x3& sourceRotation,
                         const Vector3x1& detectorPosition,
                         const Matrix3x3& detectorRotation,
                         const std::string& name) -> GenericGantry
        {
            return {
                sourcePosition,
                sourceRotation,
                detectorPosition,
                detectorRotation,
                QString::fromStdString(name)
            };
        }), "source_position"_a, "source_rotation"_a, "detector_position"_a, "detector_rotation"_a,
            "name"_a = GenericGantry::defaultName().toStdString())
        .def("info", [](const GenericGantry& self) { return self.info().toStdString(); })
        .def("set_detector_location", &GenericGantry::setDetectorLocation, "location"_a)
        .def("set_source_location", &GenericGantry::setSourceLocation, "location"_a)
        .def("set_detector_position", &GenericGantry::setDetectorPosition, "position"_a)
        .def("set_detector_rotation", &GenericGantry::setDetectorRotation, "rotation"_a)
        .def("set_source_position", &GenericGantry::setSourcePosition, "position"_a)
        .def("set_source_rotation", &GenericGantry::setSourceRotation, "rotation"_a)
        .def_static("default_name", [](){ return GenericGantry::defaultName().toStdString(); });
}
