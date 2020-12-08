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

    # use fdk to reconstruct the volume
    reconstructor = ctl.ocl.FDKReconstructor()
    reco = ctl.VoxelVolumeF.cube(50, 1.0, 0.0)
    reconstructor.configure_and_reconstruct_to(setup, projections, reco)

    # plot the reconstruction
    ctl.gui.plot(reco)
    ctl.gui.show()


if __name__ == "__main__":
    main()
