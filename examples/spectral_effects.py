import ctl
from matplotlib import pyplot as plt

def main():
    # define volume as a ball filled with attenuation 0.081/mm (approx. bone @ 50 keV)
    volume = ctl.VoxelVolumeF.ball(50, 0.5, 0.081)

    # create a spectral volume using the voxel data from volume and
    # the correct attenuation model (for bone)
    spectral_vol = ctl.SpectralVolumeData.from_mu_volume(volume, ctl.database.attenuation_model(
        ctl.database.Composite.Bone_Cortical))

    system = ctl.CTSystemBuilder.create_from_blueprint(
        ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4))
    acquisition_setup = ctl.AcquisitionSetup(system, 10)
    acquisition_setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    simple_projector = ctl.ocl.RayCasterProjector() # our simple projector

    # optional parameter settings for the projector
    # e.g. simple_projector.settings().ray_sampling = 0.1

    # This is what we do without the extension:
    # simple_projector.configure(acquisition_setup)
    # projections = simple_projector.project(volume)
    # print(projections.min(), projections.max()) # output: 0 2.79263

    # Instead we now do the following:
    extension = ctl.SpectralEffectsExtension()

    extension.use(simple_projector)                  # tell the extension to use the ray caster
    extension.set_spectral_sampling_resolution(10.0) # set the energy resolution
    extension.configure(acquisition_setup)           # configure the simulation

    # (compute and) get the final projections with and w/o spectral effects
    spectral_projections = extension.project(spectral_vol)
    projections = extension.project(volume)

    # plot differences
    proj = projections.view(0).module(0).numpy()
    spectral_proj = spectral_projections.view(0).module(0).numpy()
    _ = plt.plot(proj[proj.shape[0]//2])
    _ = plt.plot(spectral_proj[proj.shape[0]//2])
    plt.show()

if __name__ == '__main__':
    main()
