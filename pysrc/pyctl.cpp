#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>

namespace py = pybind11;

void init_ocl(py::module&);
void init_acquisitionsetup(py::module&);
void init_ctsystem(py::module&);
void init_ctsystembuilder(py::module&);
void init_trajectories(py::module&);
void init_abstractpreparestep(py::module&);

void init_chunk2d(py::module&);
void init_singleviewdata(py::module&);
void init_projectiondata(py::module&);
void init_voxelvolume(py::module&);
void init_spectralvolumedata(py::module&);

void init_abstractprojector(py::module&);

PYBIND11_MODULE(_ctl, m) {
    init_abstractprojector(m);
    init_ocl(m.def_submodule("ocl"));

    init_acquisitionsetup(m);
    init_ctsystem(m);
    init_ctsystembuilder(m);
    init_abstractpreparestep(m);
    init_trajectories(m);

    init_chunk2d(m);
    init_singleviewdata(m);
    init_projectiondata(m);
    init_voxelvolume(m);
    init_spectralvolumedata(m);
}
