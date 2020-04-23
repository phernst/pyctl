#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <components/cylindricaldetector.h>

namespace py = pybind11;

void init_cylindricaldetector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<CylindricalDetector, AbstractDetector, std::shared_ptr<CylindricalDetector>>(m, "CylindricalDetector")
        .def(py::init([](py::tuple ppm,
                         py::tuple pd,
                         uint dm,
                         double apm,
                         double ms,
                         const std::string& name) -> CylindricalDetector
        {
            if (ppm.size() != 2)
            {
                throw std::invalid_argument("nb_pixel_per_module must be a tuple of 2 elements");
            }
            if (pd.size() != 2)
            {
                throw std::invalid_argument("pixel_dimensions must be a tuple of 2 elements");
            }
            return {
                { ppm[0].cast<int>(), ppm[1].cast<int>() },
                { pd[0].cast<double>(), pd[1].cast<double>() },
                dm, apm, ms, QString::fromStdString(name)
            };
        }), "nb_pixel_per_module"_a, "pixel_dimensions"_a, "nb_detector_modules"_a,
            "angulation_per_module"_a, "module_spacing"_a,
            "name"_a = CylindricalDetector::defaultName().toStdString())
        .def_static("from_angulation_and_spacing", [](py::tuple ppm,
                         py::tuple pd,
                         uint dm,
                         double apm,
                         double ms,
                         const std::string& name)
        {
            if (ppm.size() != 2)
            {
                throw std::invalid_argument("nb_pixel_per_module must be a tuple of 2 elements");
            }
            if (pd.size() != 2)
            {
                throw std::invalid_argument("pixel_dimensions must be a tuple of 2 elements");
            }
            return CylindricalDetector::fromAngulationAndSpacing(
                { ppm[0].cast<int>(), ppm[1].cast<int>() },
                { pd[0].cast<double>(), pd[1].cast<double>() },
                dm, apm, ms, QString::fromStdString(name)
            );
        }, "nb_pixel_per_module"_a, "pixel_dimensions"_a, "nb_detector_modules"_a,
           "angulation_per_module"_a, "module_spacing"_a,
           "name"_a = CylindricalDetector::defaultName().toStdString())
        .def_static("from_radius_and_fan_angle", [](py::tuple ppm,
                         py::tuple pd,
                         uint dm,
                         double radius,
                         double fanangle,
                         const std::string& name)
        {
            if (ppm.size() != 2)
            {
                throw std::invalid_argument("nb_pixel_per_module must be a tuple of 2 elements");
            }
            if (pd.size() != 2)
            {
                throw std::invalid_argument("pixel_dimensions must be a tuple of 2 elements");
            }
            return CylindricalDetector::fromAngulationAndSpacing(
                { ppm[0].cast<int>(), ppm[1].cast<int>() },
                { pd[0].cast<double>(), pd[1].cast<double>() },
                dm, radius, fanangle, QString::fromStdString(name)
            );
        }, "nb_pixel_per_module"_a, "pixel_dimensions"_a, "nb_detector_modules"_a,
           "radius"_a, "fan_angle"_a,
           "name"_a = CylindricalDetector::defaultName().toStdString())
        .def("info", [](const CylindricalDetector& self)
        {
            return self.info().toStdString();
        })
        .def("angulation_of_module", &CylindricalDetector::angulationOfModule, "module"_a)
        .def("module_spacing", &CylindricalDetector::moduleSpacing)
        .def("cone_angle", &CylindricalDetector::coneAngle)
        .def("curvature_radius", &CylindricalDetector::curvatureRadius)
        .def("fan_angle", &CylindricalDetector::fanAngle)
        .def("row_coverage", &CylindricalDetector::rowCoverage)
        .def_static("default_name", [](){ return CylindricalDetector::defaultName().toStdString(); })
        .def("module_locations", [](const CylindricalDetector& self)
            -> std::vector<AbstractDetector::ModuleLocation>
        {
            const auto qresult { self.moduleLocations() };
            return { std::begin(qresult), std::end(qresult) };
        });
}
