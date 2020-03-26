#include <pybind11/pybind11.h>
#include <img/spectralvolumedata.h>

namespace py = pybind11;

void init_spectralvolumedata(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<SpectralVolumeData, VoxelVolume<float>>(m, "SpectralVolumeData")
        .def(py::init<VoxelVolume<float>>(), "mu_values"_a)
        .def(py::init([](VoxelVolume<float> muValues,
                         std::shared_ptr<AbstractIntegrableDataModel> absorptionModel,
                         float referenceEnergy,
                         const std::string& materialName)
        {
            return SpectralVolumeData(muValues, absorptionModel, referenceEnergy, 
                    QString::fromStdString(materialName));
        }), "mu_values"_a, "absorption_model"_a, "reference_energy"_a, "material_name"_a = "")
        .def(py::init([](VoxelVolume<float> materialDensity,
                       std::shared_ptr<AbstractIntegrableDataModel> absorptionModel,
                       const std::string& materialName)
        {
            return SpectralVolumeData(materialDensity, absorptionModel, 
                    QString::fromStdString(materialName));
        }), "material_density"_a, "absorption_model"_a, "material_name"_a = "")
        .def("absorption_model", &SpectralVolumeData::absorptionModel)
        .def("density_volume", &SpectralVolumeData::densityVolume)
        .def("has_spectral_information", &SpectralVolumeData::hasSpectralInformation)
        .def("is_density_volume", &SpectralVolumeData::isDensityVolume)
        .def("is_mu_volume", &SpectralVolumeData::isMuVolume)
        .def("mass_attenuation_coeff", &SpectralVolumeData::massAttenuationCoeff, "at_energy"_a)
        .def("material_name", [](const SpectralVolumeData* self)
        {
            return self->materialName().toStdString();
        })
        .def("mean_mass_attenuation_coeff", &SpectralVolumeData::meanMassAttenuationCoeff,
            "center_energy"_a, "bin_width"_a)
        .def("mu_volume", static_cast<std::unique_ptr<SpectralVolumeData>(SpectralVolumeData::*)
            (float)const>(&SpectralVolumeData::muVolume), "reference_energy"_a)
        .def("mu_volume", static_cast<std::unique_ptr<SpectralVolumeData>(SpectralVolumeData::*)
            (float,float)const>(&SpectralVolumeData::muVolume), "center_energy"_a, "bin_width"_a)
        .def("reference_energy", &SpectralVolumeData::referenceEnergy)
        .def("reference_mass_attenuation_coeff", &SpectralVolumeData::referenceMassAttenuationCoeff)
        .def_static("ball", &SpectralVolumeData::ball, "radius"_a, "voxel_size"_a, "density"_a,
            "absorption_model"_a)
        .def_static("cube", &SpectralVolumeData::cube, "radius"_a, "voxel_size"_a, "density"_a,
            "absorption_model"_a)
        .def_static("cylinder_x", &SpectralVolumeData::cylinderX, "radius"_a, "height"_a,
            "voxel_size"_a, "density"_a, "absorption_model"_a)
        .def_static("cylinder_y", &SpectralVolumeData::cylinderY, "radius"_a, "height"_a,
            "voxel_size"_a, "density"_a, "absorption_model"_a)
        .def_static("cylinder_z", &SpectralVolumeData::cylinderZ, "radius"_a, "height"_a,
            "voxel_size"_a, "density"_a, "absorption_model"_a)
        .def("replace_absorption_model", static_cast<void(SpectralVolumeData::*)
            (std::shared_ptr<AbstractIntegrableDataModel>)>
            (&SpectralVolumeData::replaceAbsorptionModel), "absorption_model"_a)
        .def("set_density", &SpectralVolumeData::setDensity)
        .def("set_material_name", [](SpectralVolumeData* self, std::string& name)
        {
            self->setMaterialName(QString::fromStdString(name));
        }, "name"_a)
        .def_static("from_mu_volume", &SpectralVolumeData::fromMuVolume, "mu_values"_a,
            "absorption_model"_a, "reference_energy"_a = 50.0f)
        .def_static("from_hu_volume", &SpectralVolumeData::fromHUVolume, "hu_values"_a,
            "absorption_model"_a, "reference_energy"_a = 50.0f);    
}
