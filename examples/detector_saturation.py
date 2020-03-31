import ctl
from matplotlib import pyplot as plt

def main():
    # define volume and acquisition setup (incl. system)
    volume = ctl.VoxelVolumeF.cube(100, 1.0, 0.02)
    system = ctl.CTsystemBuilder.create_from_blueprint(
        ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4))

    # set a detector saturation model (operating in extinction domain, clamps values to [0.1, 2.5])
    saturation_model = ctl.DetectorSaturationLinearModel(0.1, 2.5)
    acquisition_setup = ctl.AcquisitionSetup(system, 10)
    acquisition_setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))
    acquisition_setup.system().detector().set_saturation_model(
        saturation_model, ctl.AbstractDetector.Extinction)

    simple_projector = ctl.ocl.RayCasterProjector() # our simple projector

    # optional parameter settings for the projector
    # e.g. simple_projector.settings().ray_sampling = 0.1

    # This is what we do without the extension:
    # simple_projector.configure(acquisition_setup)
    # projections = simple_projector.project(volume)
    # print(projections.min(), projections.max()) # output: 0 2.79263

    # Instead we now do the following:
    extension = ctl.DetectorSaturationExtension()

    extension.use(simple_projector)             # tell the extension to use the ray caster
    extension.configure(acquisition_setup)      # configure the simulation

    projections = extension.project(volume)     # (compute and) get the final projections

    print(projections.min(), projections.max()) # output: 0.1 2.5

    # show projection #0
    proj = projections.view(0).module(0).to_numpy()
    _ = plt.imshow(proj, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
