#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_mat(py::module&);
void init_coordinates(py::module&);

void init_ocl(py::module&);
void init_acquisitionsetup(py::module&);
void init_systemcomponent(py::module&);
void init_abstractbeammodifier(py::module&);
void init_abstractdetector(py::module&);
void init_abstractgantry(py::module&);
void init_abstractsource(py::module&);
void init_attenuationfilter(py::module&);
void init_carmgantry(py::module&);
void init_cylindricaldetector(py::module&);
void init_flatpaneldetector(py::module&);
void init_genericbeammodifier(py::module&);
void init_genericdetector(py::module&);
void init_genericgantry(py::module&);
void init_genericsource(py::module&);
void init_tubulargantry(py::module&);
void init_xraytube(py::module&);
void init_xraylaser(py::module&);
void init_ctsystem(py::module&);
void init_simplectsystem(py::module&);
void init_ctsystembuilder(py::module&);
void init_systemblueprints(py::module&);
void init_trajectories(py::module&);
void init_abstractpreparestep(py::module&);
void init_preparesteps(py::module&);
void init_preparationprotocols(py::module&);

void init_ctldatabase(py::module&);
void init_abstractdatamodel(py::module&);
void init_detectorsaturationmodels(py::module&);

void init_chunk2d(py::module&);
void init_compositevolume(py::module&);
void init_singleviewdata(py::module&);
void init_projectiondata(py::module&);
void init_voxelvolume(py::module&);
void init_spectralvolumedata(py::module&);

void init_abstractprojector(py::module&);
void init_projectorextension(py::module&);
void init_arealfocalspotextension(py::module&);
void init_detectorsaturationextension(py::module&);
void init_dynamicprojectorextension(py::module&);
void init_poissonnoiseextension(py::module&);
void init_spectraleffectsextension(py::module&);
void init_projectionpipeline(py::module&);
void init_standardpipeline(py::module&);

PYBIND11_MODULE(_ctl, m) {
    auto protocols { m.def_submodule("protocols") };

    init_mat(m.def_submodule("mat"));
    init_coordinates(m);
    init_abstractprojector(m);
    init_ctldatabase(m);
    init_abstractdatamodel(m);
    init_detectorsaturationmodels(m);
    init_ocl(m.def_submodule("ocl"));
    init_projectorextension(m);
    init_arealfocalspotextension(m);
    init_detectorsaturationextension(m);
    init_dynamicprojectorextension(m);
    init_poissonnoiseextension(m);
    init_spectraleffectsextension(m);
    init_projectionpipeline(m);
    init_standardpipeline(m);

    init_acquisitionsetup(m);
    init_systemcomponent(m);
    init_abstractbeammodifier(m);
    init_abstractdetector(m);
    init_abstractgantry(m);
    init_abstractsource(m);
    init_attenuationfilter(m);
    init_carmgantry(m);
    init_cylindricaldetector(m);
    init_flatpaneldetector(m);
    init_genericbeammodifier(m);
    init_genericdetector(m);
    init_genericgantry(m);
    init_genericsource(m);
    init_tubulargantry(m);
    init_xraytube(m);
    init_xraylaser(m);
    init_ctsystem(m);
    init_simplectsystem(m);
    init_ctsystembuilder(m);
    init_systemblueprints(m);
    init_abstractpreparestep(m);
    init_preparesteps(m);
    init_preparationprotocols(protocols);
    init_trajectories(protocols);

    init_chunk2d(m);
    init_compositevolume(m);
    init_singleviewdata(m);
    init_projectiondata(m);
    init_voxelvolume(m);
    init_spectralvolumedata(m);
}
