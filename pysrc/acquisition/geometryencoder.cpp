#include "pysrc/pybind_common.h"
#include <acquisition/geometryencoder.h>

namespace py = pybind11;

void init_geometryencoder(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;
    
    py::class_<GeometryEncoder>(m, "GeometryEncoder")
        // TODO - pybind11: Don't allow mixed static/non-static overloads #804
        // .def(py::init<const SimpleCTSystem*>(), "system"_a)
        // .def("assign_system", &GeometryEncoder::assignSystem, "system"_a, py::keep_alive<1,2>())
        // .def("encode_single_view_geometry", [](const GeometryEncoder& self)
        // {
        //     return self.encodeSingleViewGeometry();
        // })
        // .def("system", &GeometryEncoder::system)
        // .def("effective_pixel_area", [](const GeometryEncoder& self, uint module)
        // {
        //     return self.effectivePixelArea(module);
        // }, "module"_a)
        // .def("effective_pixel_areas", [](const GeometryEncoder& self)
        // {
        //     return self.effectivePixelAreas();
        // })
        // .def("final_module_position", [](const GeometryEncoder& self, uint module)
        // {
        //     return self.finalModulePosition(module);
        // }, "module"_a)
        // .def("final_module_rotation", [](const GeometryEncoder& self, uint module)
        // {
        //     return self.finalModuleRotation(module);
        // }, "module"_a)
        // .def("final_source_position", [](const GeometryEncoder& self)
        // {
        //     return self.finalSourcePosition();
        // })
        .def_static("encode_full_geometry", [](AcquisitionSetup setup)
        {
            return GeometryEncoder::encodeFullGeometry(setup);
        }, "setup"_a)
        .def_static("encode_single_view_geometry", [](const SimpleCTSystem& system)
        {
            return GeometryEncoder::encodeSingleViewGeometry(system);
        }, "system"_a)
        .def_static("effective_pixel_area", [](const SimpleCTSystem& system, uint module)
        {
            return GeometryEncoder::effectivePixelArea(system, module);
        }, "system"_a, "module"_a)
        .def_static("effective_pixel_areas", [](const SimpleCTSystem& system)
        {
            return GeometryEncoder::effectivePixelAreas(system);
        }, "system"_a)
        .def_static("final_module_position", [](const SimpleCTSystem& system, uint module)
        {
            return GeometryEncoder::finalModulePosition(system, module);
        }, "system"_a, "module"_a)
        .def_static("final_module_rotation", [](const SimpleCTSystem& system, uint module)
        {
            return GeometryEncoder::finalModuleRotation(system, module);
        }, "system"_a, "module"_a)
        .def_static("final_source_position", [](const SimpleCTSystem& system)
        {
            return GeometryEncoder::finalSourcePosition(system);
        }, "system"_a);
}
