#include <pybind11/pybind11.h>
#include <models/abstractdatamodel.h>

namespace py = pybind11;

void init_abstractdatamodel(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractDataModel, std::shared_ptr<AbstractDataModel>>(m, "AbstractDataModel")
        .def("is_integrable", &AbstractDataModel::isIntegrable)
        .def("name", [](const AbstractDataModel* self)
        {
            return self->name().toStdString();
        })
        .def("set_name", [](AbstractDataModel* self, const std::string& name)
        {
            self->setName(QString::fromStdString(name));
        }, "name"_a);

    py::class_<AbstractIntegrableDataModel, AbstractDataModel, std::shared_ptr<AbstractIntegrableDataModel>>(m, "AbstractIntegrableDataModel")
        .def("mean_value", &AbstractIntegrableDataModel::meanValue, "position"_a, "bin_width"_a);
}
