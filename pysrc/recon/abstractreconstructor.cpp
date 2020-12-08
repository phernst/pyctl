#include <pybind11/pybind11.h>
#include <recon/abstractreconstructor.h>
#include <img/voxelvolume.h>
#include <img/sparsevoxelvolume.h>
#include <img/compositevolume.h>
#include <img/abstractdynamicvolumedata.h>
#include <img/spectralvolumedata.h>
#include <acquisition/acquisitionsetup.h>
#include "../pysharedptr.h"

namespace py = pybind11;

namespace
{

using namespace py::literals;

template <class VolumeType>
void add_reconstructTo(py::class_<CTL::AbstractReconstructor, PySharedPtr<CTL::AbstractReconstructor>>& recon)
{
    recon.def("reconstruct_to", &CTL::AbstractReconstructor::reconstructTo<VolumeType>,
        "projections"_a, "target_volume"_a);
}

template <class VolumeType>
void add_configureAndReconstructTo(py::class_<CTL::AbstractReconstructor, PySharedPtr<CTL::AbstractReconstructor>>& recon)
{
    recon.def("configure_and_reconstruct_to",
        &CTL::AbstractReconstructor::configureAndReconstructTo<VolumeType>,
        "setup"_a, "projections"_a, "target_volume"_a);
}

template <class VolumeType>
void add_reconstruct(py::class_<CTL::AbstractReconstructor, PySharedPtr<CTL::AbstractReconstructor>>& recon)
{
    recon.def("reconstruct", [](CTL::AbstractReconstructor& self,
                                const CTL::ProjectionDataView& projections,
                                const VolumeType& initialVolume)
    {
        return self.reconstruct(projections, initialVolume);
    }, "projections"_a, "target_volume"_a);
}

template <class VolumeType>
void add_configureAndReconstruct(py::class_<CTL::AbstractReconstructor, PySharedPtr<CTL::AbstractReconstructor>>& recon)
{
    recon.def("configure_and_reconstruct", [](CTL::AbstractReconstructor& self,
                                              const CTL::AcquisitionSetup& setup,
                                              const CTL::ProjectionDataView& projections,
                                              const VolumeType& initialVolume)
    {
        return self.configureAndReconstruct(setup, projections, initialVolume);
    }, "setup"_a, "projections"_a, "target_volume"_a);
}
}

void init_abstractreconstructor(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto ar = py::class_<AbstractReconstructor, PySharedPtr<AbstractReconstructor>>(m, "AbstractReconstructor");
    ar.def("is_applicable_to", &AbstractReconstructor::isApplicableTo, "setup"_a)
        .def("reconstruct_to_plain", &AbstractReconstructor::reconstructToPlain, "projections"_a,
            "target_volume"_a)
        .def("reconstruct_to_spectral", &AbstractReconstructor::reconstructToSpectral,
            "projections"_a, "target_volume"_a)
        .def("reconstruct_to_dynamic", &AbstractReconstructor::reconstructToDynamic,
            "projections"_a, "target_volume"_a)
        .def("reconstruct_to_composite", &AbstractReconstructor::reconstructToComposite,
            "projections"_a, "target_volume"_a)
        .def("reconstruct_to_sparse", &AbstractReconstructor::reconstructToSparse, "projections"_a,
            "target_volume"_a);
    
    add_reconstructTo<VoxelVolume<float>>(ar);
    add_reconstructTo<SpectralVolumeData>(ar);
    add_reconstructTo<AbstractDynamicVolumeData>(ar);
    add_reconstructTo<CompositeVolume>(ar);
    add_reconstructTo<SparseVoxelVolume>(ar);

    add_configureAndReconstructTo<VoxelVolume<float>>(ar);
    add_configureAndReconstructTo<SpectralVolumeData>(ar);
    add_configureAndReconstructTo<AbstractDynamicVolumeData>(ar);
    add_configureAndReconstructTo<CompositeVolume>(ar);
    add_configureAndReconstructTo<SparseVoxelVolume>(ar);

    add_reconstruct<VoxelVolume<float>>(ar);
    add_reconstruct<SpectralVolumeData>(ar);
    // add_reconstruct<AbstractDynamicVolumeData>(ar); // TODO?
    add_reconstruct<CompositeVolume>(ar);
    add_reconstruct<SparseVoxelVolume>(ar);

    add_configureAndReconstruct<VoxelVolume<float>>(ar);
    add_configureAndReconstruct<SpectralVolumeData>(ar);
    // add_configureAndReconstruct<AbstractDynamicVolumeData>(ar); // TODO?
    add_configureAndReconstruct<CompositeVolume>(ar);
    add_configureAndReconstruct<SparseVoxelVolume>(ar);
}
