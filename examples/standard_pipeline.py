import ctl
from matplotlib import pyplot as plt


def main():
    # create a water ball
    volume = ctl.SpectralVolumeData.ball(50., 0.5, 1.0, ctl.database.attenuation_model(
        ctl.database.Composite.Water))

    # create a C-arm CT system and a short scan protocol with 10 views
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericCarmCT())
    setup = ctl.AcquisitionSetup(system, 10)
    setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    # create the standard pipeline and adjust the desired settings (focal spot & energy resolution)
    pipe = ctl.StandardPipeline()
    pipe.enable_areal_focal_spot()
    pipe.settings_spectral_effects().set_sampling_resolution(5.0)

    # pass the acquisition setup and run the simulation
    projections = pipe.configure_and_project(setup, volume)

    # show projection #1
    proj = projections.view(1).module(0).numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()


if __name__ == '__main__':
    main()
