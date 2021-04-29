#include "pysrc/pybind_common.h"
#include <acquisition/simplectsystem.h>
#include <components/abstractdetector.h>
#include <components/abstractgantry.h>
#include <components/abstractsource.h>
#include <components/abstractbeammodifier.h>

namespace py = pybind11;

void init_simplectsystem(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    py::class_<SimpleCTSystem, CTSystem>(m, "SimpleCTSystem")
        .def(py::init<const AbstractDetector&,const AbstractGantry&,const AbstractSource&>(),
            "detector"_a, "gantry"_a, "source"_a)
        .def_static("from_ctsystem", [](const CTSystem& system)
        {
            return SimpleCTSystem::fromCTSystem(system);
        })
        .def("detector", &SimpleCTSystem::detector, rvp::reference)
        .def("gantry", &SimpleCTSystem::gantry, rvp::reference)
        .def("source", &SimpleCTSystem::source, rvp::reference)
        .def("replace_detector", [](SimpleCTSystem& self, const AbstractDetector& det)
        {
            self.replaceDetector(static_cast<AbstractDetector*>(det.clone()));
        }, "new_detector"_a)
        .def("replace_gantry", [](SimpleCTSystem& self, const AbstractGantry& gan)
        {
            self.replaceGantry(static_cast<AbstractGantry*>(gan.clone()));
        }, "new_gantry"_a)
        .def("replace_source", [](SimpleCTSystem& self, const AbstractSource& src)
        {
            self.replaceSource(static_cast<AbstractSource*>(src.clone()));
        }, "new_source"_a)
        .def("add_beam_modifier", [](SimpleCTSystem& self, const AbstractBeamModifier& bm)
        {
            self.addBeamModifier(static_cast<AbstractBeamModifier*>(bm.clone()));
        }, "modifier"_a)
        .def("photons_per_pixel_mean", &SimpleCTSystem::photonsPerPixelMean)
        .def("photons_per_pixel", [](const SimpleCTSystem& self, uint m)
        {
            return self.photonsPerPixel(m);
        }, "module"_a)
        .def("photons_per_pixel", [](const SimpleCTSystem& self)
        {
            return self.photonsPerPixel();
        });
}