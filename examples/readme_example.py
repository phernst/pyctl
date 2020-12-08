import sys

import ctl
import ctl.gui
from matplotlib import pyplot as plt

def main():
    # create a cylinder as a volume
    volume = ctl.VoxelVolumeF.cylinder_x(radius=60.0,
                                         height=100.0,
                                         voxel_size=0.5,
                                         fill_value=0.03)

    # alternatively:
    # volume = ctl.VoxelVolumeF.from_numpy(np.ones((128, 128, 128)))
    # volume.set_voxel_size((1.0, 1.0, 1.0))

    # use of a predefined system from ctl.blueprints
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericCarmCT())

    # create an acquisition setup
    nb_views = 100
    my_carm_setup = ctl.AcquisitionSetup(system, nb_views)

    # add a predefined trajectory to the setup from ctl.protocols
    source_to_isocenter = 750.0 # mm is the standard unit for length dimensions
    start_angle = ctl.deg2rad(42.0) # rad is the standard unit for angles
    my_carm_setup.apply_preparation_protocol(ctl.protocols.WobbleTrajectory(source_to_isocenter,
                                                                            start_angle))

    if not my_carm_setup.is_valid():
        sys.exit(-1)

    # configure a projector and project volume
    my_projector = ctl.ocl.RayCasterProjector() # an ideal projector with default settings
    projections = my_projector.configure_and_project(my_carm_setup, volume)

    # plot the projections
    ctl.gui.plot(projections)
    ctl.gui.show()

    # show the 20th projection of detector module 0 using numpy
    proj20 = projections.numpy()[20, 0]
    # alternatively: proj20 = projections.view(20).module(0).numpy()
    _ = plt.imshow(proj20, cmap='gray'), plt.show()

if __name__ == '__main__':
    main()
