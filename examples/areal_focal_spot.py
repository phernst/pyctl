import ctl
from matplotlib import pyplot as plt
import numpy as np

def main():
    # define volume and acquisition setup (incl. system)
    volume = ctl.VoxelVolumeF.cube(100, 1.0, 0.02)
    system = ctl.CTsystemBuilder.create_from_blueprint(
        ctl.blueprints.GenericCarmCT(ctl.DetectorBinning.Binning4x4))

    # create a simple short scan setup with 10 views
    acquisition_setup = ctl.AcquisitionSetup(system, 10)
    acquisition_setup.apply_preparation_protocol(ctl.protocols.ShortScanTrajectory(750.0))

    simple_projector = ctl.ocl.RayCasterProjector() # our simple projector

    # optional parameter settings for the projector
    # e.g. simple_projector.settings().ray_sampling = 0.1

    # This is what we do without the extension:
    # simple_projector.configure(acquisition_setup)
    # projections = simple_projector.project(volume)

    # Instead we now do the following:
    extension = ctl.ArealFocalSpotExtension()

    extension.use(simple_projector)             # tell the extension to use the ray caster
    extension.set_discretization((5, 5))        # set discretization grid to 5x5 points
    extension.configure(acquisition_setup)      # configure the simulation

    projections = extension.project(volume)     # (compute and) get the final projections

    # show projection #0
    proj = projections.view(0).module(0)
    proj_dims = tuple(reversed(proj.dimensions()))
    projection_np = np.array(proj.data(), dtype=np.float).reshape(proj_dims)
    _ = plt.imshow(projection_np, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
