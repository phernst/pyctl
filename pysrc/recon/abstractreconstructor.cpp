#include <pybind11/pybind11.h>
#include <recon/abstractreconstructor.h>
#include <img/voxelvolume.h>
#include <acquisition/acquisitionsetup.h>
#include <img/compositevolume.h>
#include <img/spectralvolumedata.h>
#include <img/sparsevoxelvolume.h>

namespace py = pybind11;

void init_abstractreconstructor(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractReconstructor>(m, "AbstractReconstructor")
        .def("configure", &AbstractReconstructor::configure, "setup"_a)
        .def("is_applicable_to", &AbstractReconstructor::isApplicableTo, "setup"_a)
        .def("reconstruct", [](AbstractReconstructor& self,
                               const ProjectionDataView& pdv,
                               const VoxelVolume<float>& initVol)
        {
            return self.reconstruct(pdv, initVol);
        }, "projections"_a, "initial_volume"_a)
        .def("reconstruct", [](AbstractReconstructor& self,
                               const ProjectionDataView& pdv,
                               const CompositeVolume& initVol)
        {
            return self.reconstruct(pdv, initVol);
        }, "projections"_a, "initial_volume"_a)
        .def("reconstruct", [](AbstractReconstructor& self,
                               const ProjectionDataView& pdv,
                               const SpectralVolumeData& initVol)
        {
            return self.reconstruct(pdv, initVol);
        }, "projections"_a, "initial_volume"_a)
        .def("reconstruct", [](AbstractReconstructor& self,
                               const ProjectionDataView& pdv,
                               const SparseVoxelVolume& initVol)
        {
            return self.reconstruct(pdv, initVol);
        }, "projections"_a, "initial_volume"_a)
        .def("configure_and_reconstruct", [](AbstractReconstructor& self,
                                             const AcquisitionSetup& setup,
                                             const ProjectionDataView& pdv,
                                             const VoxelVolume<float>& initVol)
        {
            return self.configureAndReconstruct(setup, pdv, initVol);
        }, "setup"_a, "projections"_a, "initial_volume"_a)
        .def("configure_and_reconstruct", [](AbstractReconstructor& self,
                                             const AcquisitionSetup& setup,
                                             const ProjectionDataView& pdv,
                                             const CompositeVolume& initVol)
        {
            return self.configureAndReconstruct(setup, pdv, initVol);
        }, "setup"_a, "projections"_a, "initial_volume"_a)
        .def("configure_and_reconstruct", [](AbstractReconstructor& self,
                                             const AcquisitionSetup& setup,
                                             const ProjectionDataView& pdv,
                                             const SpectralVolumeData& initVol)
        {
            return self.configureAndReconstruct(setup, pdv, initVol);
        }, "setup"_a, "projections"_a, "initial_volume"_a)
        .def("configure_and_reconstruct", [](AbstractReconstructor& self,
                                             const AcquisitionSetup& setup,
                                             const ProjectionDataView& pdv,
                                             const SparseVoxelVolume& initVol)
        {
            return self.configureAndReconstruct(setup, pdv, initVol);
        }, "setup"_a, "projections"_a, "initial_volume"_a);

}
