#include <pybind11/pybind11.h>
#include <projectors/raycasterprojector.h>
#include <components/abstractdetector.h>

namespace py = pybind11;

void init_raycasterprojector(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto rcp = py::class_<OCL::RayCasterProjector, AbstractProjector>(m, "RayCasterProjector")
        .def(py::init<>())
        .def("configure", &OCL::RayCasterProjector::configure, "setup"_a)
        .def("project", &OCL::RayCasterProjector::project, "volume"_a)
        .def("project", [](OCL::RayCasterProjector* self, const VoxelVolume<float>& volume)
        {
            return self->project(volume);
        }, "volume"_a)
        .def("settings", &OCL::RayCasterProjector::settings);

    py::class_<OCL::RayCasterProjector::Settings>(rcp, "Settings")
        .def(py::init<>())
        .def_static("optimized_for", &OCL::RayCasterProjector::Settings::optimizedFor,
            "volume"_a, "detector"_a)
        .def_readwrite("device_ids", &OCL::RayCasterProjector::Settings::deviceIDs)
        .def_property("rays_per_pixel", [](const OCL::RayCasterProjector::Settings* self)
        {
            auto t { py::tuple(2) };
            t[0] = self->raysPerPixel[0];
            t[1] = self->raysPerPixel[1];
            return t;
        }, [](OCL::RayCasterProjector::Settings* self, py::tuple t)
        {
            self->raysPerPixel[0] = t[0].cast<uint>();
            self->raysPerPixel[1] = t[1].cast<uint>();
        })
        .def_readwrite("ray_sampling", &OCL::RayCasterProjector::Settings::raySampling)
        .def_readwrite("volume_upsampling", &OCL::RayCasterProjector::Settings::volumeUpSampling)
        .def_readwrite("interpolate", &OCL::RayCasterProjector::Settings::interpolate);
}
