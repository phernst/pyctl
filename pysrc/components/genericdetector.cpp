#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <components/genericdetector.h>

namespace py = pybind11;

void init_genericdetector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;
    using ModLoc = GenericDetector::ModuleLocation;

    // TODO - holder type necessary?
    py::class_<GenericDetector, AbstractDetector, std::shared_ptr<GenericDetector>>(m, "GenericDetector")
        .def("module_locations", [](const GenericDetector& self) -> std::vector<ModLoc>
        {
            const auto qlocations { self.moduleLocations() };
            return { std::begin(qlocations), std::end(qlocations) };
        })
        .def(py::init([](py::tuple nbPixelPerModule,
                         uint nbModules,
                         const std::string& name) -> GenericDetector
        {
            if (nbPixelPerModule.size() != 2)
            {
                throw std::invalid_argument("nb_pixel_per_module must be a tuple of 2 elements");
            }
            return {
                { nbPixelPerModule[0].cast<int>(), nbPixelPerModule[1].cast<int>() },
                nbModules,
                QString::fromStdString(name)
            };
        }), "nb_pixel_per_module"_a, "nb_modules"_a,
            "name"_a = GenericDetector::defaultName().toStdString())
        .def(py::init([](py::tuple nbPixelPerModule,
                         py::tuple pixelDimensions,
                         const std::vector<ModLoc>& moduleLocations,
                         const std::string& name) -> GenericDetector
        {
            if (nbPixelPerModule.size() != 2)
            {
                throw std::invalid_argument("nb_pixel_per_module must be a tuple of 2 elements");
            }
            if (pixelDimensions.size() != 2)
            {
                throw std::invalid_argument("pixel_dimensions must be a tuple of 2 elements");
            }
            // TODO: don't use fromStdVector
            return {
                { nbPixelPerModule[0].cast<int>(), nbPixelPerModule[1].cast<int>() },
                { pixelDimensions[0].cast<double>(), pixelDimensions[1].cast<double>() },
                QVector<ModLoc>::fromStdVector(moduleLocations),
                QString::fromStdString(name)
            };
        }), "nb_pixel_per_module"_a, "pixel_dimensions"_a, "module_locations"_a,
            "name"_a = GenericDetector::defaultName().toStdString())
        .def("info", [](const GenericDetector& self) { return self.info().toStdString(); })
        .def("set_module_locations", [](GenericDetector& self, const std::vector<ModLoc>& ml)
        {
            // TODO: don't use fromStdVector
            self.setModuleLocations(QVector<ModLoc>::fromStdVector(ml));
        }, "module_locations"_a)
        .def("set_pixel_size", [](GenericDetector& self, py::tuple size)
        {
            self.setPixelSize({ size[0].cast<double>(), size[1].cast<double>() });
        }, "size"_a)
        .def("set_skew_angle", &GenericDetector::setSkewAngle, "skew_angle"_a)
        .def_static("default_name", [](){ return GenericDetector::defaultName().toStdString(); });
}
