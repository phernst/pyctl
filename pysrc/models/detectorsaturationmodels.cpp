#include <pybind11/pybind11.h>
#include <models/detectorsaturationmodels.h>

namespace py = pybind11;

void init_detectorsaturationmodels(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<DetectorSaturationLinearModel, AbstractDataModel, std::shared_ptr<DetectorSaturationLinearModel>>(m, "DetectorSaturationLinearModel")
        .def(py::init<float, float>(), "lower_cap"_a = 0.0f, "upper_cap"_a = FLT_MAX)
        .def("value_at", &DetectorSaturationLinearModel::valueAt, "position"_a);

    py::class_<DetectorSaturationSplineModel, AbstractDataModel, std::shared_ptr<DetectorSaturationSplineModel>>(m, "DetectorSaturationSplineModel")
        .def(py::init<float, float, float>(), "lower_cap"_a = 0.0f, "upper_cap"_a = FLT_MAX,
            "softening"_a = 0.0f)
        .def(py::init<float, float, float, float>(), "lower_cap"_a, "upper_cap"_a, "soft_lower"_a,
            "soft_upper"_a)
        .def("value_at", &DetectorSaturationSplineModel::valueAt, "position"_a);
}
