#include "pysrc/pybind_common.h"
#include <projectors/raycasterprojectorcpu.h>
#include <components/abstractdetector.h>
#include <acquisition/acquisitionsetup.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_raycasterprojectorcpu(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto rcp = py::class_<RayCasterProjectorCPU, AbstractProjector, PySharedPtr<RayCasterProjectorCPU>>(m, "RayCasterProjectorCPU")
        .def(py::init<>())
        .def("configure", &RayCasterProjectorCPU::configure, "setup"_a, py::keep_alive<1,2>())
        .def("project", &RayCasterProjectorCPU::project, "volume"_a)
        .def("settings", &RayCasterProjectorCPU::settings);

    using Settings = std::remove_reference<decltype(RayCasterProjectorCPU().settings())>::type;

    py::class_<Settings>(rcp, "Settings")
        .def(py::init<>())
        .def_property("rays_per_pixel", [](const Settings& self)
        {
            auto t { py::tuple(2) };
            t[0] = self.raysPerPixel[0];
            t[1] = self.raysPerPixel[1];
            return t;
        }, [](Settings& self, py::tuple t)
        {
            self.raysPerPixel[0] = t[0].cast<uint>();
            self.raysPerPixel[1] = t[1].cast<uint>();
        })
        .def_readwrite("ray_sampling", &Settings::raySampling)
        .def_readwrite("interpolate", &Settings::interpolate);
}
