import ctl
from matplotlib import pyplot as plt

def main():
    # Our starting volume remains the same. This is the sub-volume without spectral information.
    volume = ctl.CompositeVolume(ctl.VoxelVolumeF.cube(150, 1.0, 0.02))

    # We now create two balls with spectral information (one representing blood, the other bone).
    sub_volume_1 = ctl.SpectralVolumeData.ball(10.0, 1.0, 0.05, ctl.database.attenuation_model(
        ctl.database.Composite.Blood))
    sub_volume_2 = ctl.SpectralVolumeData.ball(25.0, 1.0, 0.10, ctl.database.attenuation_model(
        ctl.database.Composite.Bone_Cortical))

    # Again, the shift to the desired positions...
    sub_volume_1.set_volume_offset((0.0, -20.0, 0.0))
    sub_volume_2.set_volume_offset((0.0, 30.0, 0.0))

    # ... and adding to the final volume.
    volume.add_sub_volume(sub_volume_1)
    volume.add_sub_volume(sub_volume_2)

    # In the projection code, we only change the setting for the
    # standard pipeline to 'No_Approximation'...
    setup = ctl.AcquisitionSetup(
        ctl.CTSystemBuilder.create_from_blueprint(
            ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4)),
        10)
    setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    # ... here comes the changed line:
    projector = ctl.StandardPipeline(ctl.StandardPipeline.No_Approximation)

    # Pass the acquisition setup to the projector and create the projections:
    projector.configure(setup)
    projections = projector.project_composite(volume)

    # show projection #0
    proj = projections.view(0).module(0).numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
