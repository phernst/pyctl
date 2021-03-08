import ctl
import ctl.gui


def main():
    # create a volume to project and reconstruct
    volume = ctl.VoxelVolumeF.cube(10, 1.0, 0.02)

    # build a c-arm setup with 90 views and a short scan trajectory
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericCarmCT())
    setup = ctl.AcquisitionSetup(system, 90)
    setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(500.))

    # project the volume
    projector = ctl.ocl.RayCasterProjector()
    projections = projector.configure_and_project(setup, volume)

    # use art to reconstruct the volume, set some constraints
    reconstructor = ctl.ARTReconstructor()
    reconstructor.set_positivity_constraint_enabled(True)
    reconstructor.set_relaxation_estimation_enabled(True)
    reconstructor.set_min_relative_projection_error(3e-2)
    reconstructor.set_max_nb_iterations(20)

    # use a modified subset generator
    subset_gen = ctl.DefaultSubsetGenerator()
    subset_gen.set_order(ctl.DefaultSubsetGenerator.Orthogonal180)
    reconstructor.set_subset_generator(subset_gen)

    # reconstruct
    reco = ctl.VoxelVolumeF.cube(100, 0.5, 0.0)
    reconstructor.configure_and_reconstruct_to(setup, projections, reco)

    # output:
    # "IterativeReconstructor: Terminated due relative projection error below tolerance level
    # (rel. error: 0.029831 | threshold: 0.03)."

    print(f'estimated relaxation: {reconstructor.relaxation()}')

    # plot the reconstruction
    ctl.gui.plot(reco)
    ctl.gui.show()


if __name__ == "__main__":
    main()
