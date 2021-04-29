#include "pysrc/pybind_common.h"
#include <components/flatpaneldetector.h>

namespace py = pybind11;

void init_flatpaneldetector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    // TODO - holder type necessary?
    py::class_<FlatPanelDetector, AbstractDetector, std::shared_ptr<FlatPanelDetector>>(m, "FlatPanelDetector")
        .def(py::init([](py::tuple ppm,
                         py::tuple pd,
                         const std::string& name) -> FlatPanelDetector
        {
            if (ppm.size() != 2)
            {
                throw std::invalid_argument("nb_pixels must be a tuple of 2 elements");
            }
            if (pd.size() != 2)
            {
                throw std::invalid_argument("pixel_dimensions must be a tuple of 2 elements");
            }
            return {
                { ppm[0].cast<int>(), ppm[1].cast<int>() },
                { pd[0].cast<double>(), pd[1].cast<double>() },
                QString::fromStdString(name)
            };
        }), "nb_pixels"_a, "pixel_dimensions"_a,
            "name"_a = FlatPanelDetector::defaultName().toStdString())
        .def("info", [](const FlatPanelDetector& self)
        {
            return self.info().toStdString();
        })
        .def("location", &FlatPanelDetector::location)
        .def("nb_pixels", [](const FlatPanelDetector& self)
        {
            const auto& pix { self.nbPixels() };
            auto result { py::tuple(2) };
            result[0] = pix.width();
            result[1] = pix.height();
            return result;
        })
        .def("panel_dimensions", [](const FlatPanelDetector& self)
        {
            const auto pix { self.panelDimensions() };
            auto result { py::tuple(2) };
            result[0] = pix.width();
            result[1] = pix.height();
            return result;
        })
        .def_static("default_name", [](){ return FlatPanelDetector::defaultName().toStdString(); })
        .def("module_locations", [](const FlatPanelDetector& self)
            -> std::vector<AbstractDetector::ModuleLocation>
        {
            const auto qresult { self.moduleLocations() };
            return { std::begin(qresult), std::end(qresult) };
        });
}
