import ctl
from matplotlib import pyplot as plt


def main():
    # define volume and acquisition setup (incl. system)
    volume = ctl.VoxelVolumeF.cube(100, 1.0, 0.02)
    system = ctl.SimpleCTSystem.from_ctsystem(ctl.CTSystemBuilder.create_from_blueprint(
        ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4)))

    # reduce default radiation output of the source component (-> make noise more prominent)
    system.source().set_milliampere_seconds(0.001)

    # create a simple short scan setup with 10 views
    acquisition_setup = ctl.AcquisitionSetup(system, 10)
    acquisition_setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    simple_projector = ctl.ocl.RayCasterProjector()  # our simple projector

    # optional parameter settings for the projector
    # e.g. simple_projector.settings().ray_sampling = 0.1

    # This is what we do without the extension:
    # projections = simple_projector.configure_and_project(acquisition_setup, volume)

    # Instead we now do the following:
    extension = ctl.PoissonNoiseExtension()

    extension.use(simple_projector)             # tell the extension to use the ray caster
    extension.set_fixed_seed(42)                # set discretization grid to 5x5 points

    # (compute and) get the final projections
    projections = extension.configure_and_project(acquisition_setup, volume)

    # show projection #0
    proj = projections.view(0).module(0).numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()


if __name__ == '__main__':
    main()
