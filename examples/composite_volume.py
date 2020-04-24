import ctl
from matplotlib import pyplot as plt

def main():
    # Construct the composite volume already containing the cube volume.
    volume = ctl.CompositeVolume(ctl.VoxelVolumeF.cube(150, 1.0, 0.02))

    # We now construct the two ball volumes.
    sub_volume_1 = ctl.VoxelVolumeF.ball(10.0, 1.0, 0.05)
    sub_volume_2 = ctl.VoxelVolumeF.ball(25.0, 1.0, 0.10)

    # Here, we shift the ball volumes to the desired positions.
    sub_volume_1.set_volume_offset((0.0, -20.0, 0.0))
    sub_volume_2.set_volume_offset((0.0, 30.0, 0.0))

    # Now, we add the two balls as sub-volumes to our final volume.
    volume.add_sub_volume(sub_volume_1)
    volume.add_sub_volume(sub_volume_2)

    # First, we need to define an acquisition setup
    # (with a CT system and the number of views; 10 in this case))
    setup = ctl.AcquisitionSetup(
        ctl.CTSystemBuilder.create_from_blueprint(
            ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4)),
        10)

    # We also need to specify the acquisition geometry, here we set a simple short scan trajectory
    setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    # Now, we create our projector, here we simply use the standard pipeline.
    projector = ctl.StandardPipeline()

    # Pass the acquisition setup to the projector and create the projections:
    projector.configure(setup)
    projections = projector.project_composite(volume)

    # show projection #0
    proj = projections.view(0).module(0).to_numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
