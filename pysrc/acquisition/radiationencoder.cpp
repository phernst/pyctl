#include "pysrc/pybind_common.h"
#include <acquisition/radiationencoder.h>

namespace py = pybind11;

void init_radiationencoder(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    
    py::class_<RadiationEncoder>(m, "RadiationEncoder")
        .def(py::init<const SimpleCTSystem*>(), "system"_a)
        .def("assign_system", &RadiationEncoder::assignSystem, "system"_a)
        .def("final_spectrum", static_cast<IntervalDataSeries(
            RadiationEncoder::*)(uint)const>(&RadiationEncoder::finalSpectrum),
            "nb_samples"_a)
        .def("final_spectrum", static_cast<IntervalDataSeries(
            RadiationEncoder::*)(EnergyRange, uint)const>(
            &RadiationEncoder::finalSpectrum), "range"_a, "nb_samples"_a)
        .def("final_photon_flux", &RadiationEncoder::finalPhotonFlux)
        .def("photons_per_pixel_mean", &RadiationEncoder::photonsPerPixelMean)
        .def("photons_per_pixel", static_cast<float(RadiationEncoder::*)(uint)
            const>(&RadiationEncoder::photonsPerPixel), "module"_a)
        .def("photons_per_pixel", static_cast<std::vector<float>(
            RadiationEncoder::*)()const>(&RadiationEncoder::photonsPerPixel))
        .def("detective_quantum_efficiency",
            &RadiationEncoder::detectiveQuantumEfficieny) // TODO: fix typo
        .def("detective_mean_energy", &RadiationEncoder::detectiveMeanEnergy)
        .def("system", &RadiationEncoder::system) // TODO: rvp?
        .def_static("spectral_information",
            &RadiationEncoder::spectralInformation, "setup"_a,
            "energy_resolution"_a = 0.0f);

    auto si = py::class_<SpectralInformation>(m, "SpectralInformation");

    py::class_<SpectralInformation::BinInformation>(si, "BinInformation")
        .def_readwrite("intensities",
            &SpectralInformation::BinInformation::intensities)
        .def_readwrite("adjusted_flux_mods",
            &SpectralInformation::BinInformation::adjustedFluxMods)
        .def_readwrite("energy",
            &SpectralInformation::BinInformation::energy);

    si.def("nb_energy_bins", &SpectralInformation::nbEnergyBins)
        .def("bin_width", &SpectralInformation::binWidth)
        .def("bin", &SpectralInformation::bin, "bin_idx"_a)
        .def("total_intensity", &SpectralInformation::totalIntensity)
        .def("full_coverage_range", &SpectralInformation::fullCoverageRange)
        .def("highest_reso", &SpectralInformation::highestReso);
}