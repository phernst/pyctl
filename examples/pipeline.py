import ctl
from matplotlib import pyplot as plt

def main():
    # create ball phantom made of cortical bone
    volume = ctl.SpectralVolumeData.ball(50., 0.5, 1.0, ctl.database.attenuation_model(
        ctl.database.Composite.Bone_Cortical))

    # create a C-arm CT system and a short scan protocol with 10 views
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericCarmCT())
    setup = ctl.AcquisitionSetup(system, 10)
    setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    # create the pipeline with a ray caster projector
    pipe = ctl.ProjectionPipeline(ctl.ocl.RayCasterProjector())

    # create a SpectralEffectsExtension and set the energy resolution to 7.5 keV
    spectral_ext = ctl.SpectralEffectsExtension()
    spectral_ext.set_spectral_sampling_resolution(7.5)

    # add the spectral effects extension and a Poisson noise extension to the pipeline
    pipe.append_extension(spectral_ext)
    pipe.append_extension(ctl.PoissonNoiseExtension())

    # pass the acquisition setup and run the simulation
    pipe.configure(setup)
    projections = pipe.project(volume)

    # show projection #1
    proj = projections.view(1).module(0).numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
