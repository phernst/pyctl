import argparse

import ctl
import nrrd

def main(path_to_nrrd):
    # load the nrrd file
    nrrd_volume, nrrd_header = nrrd.read(path_to_nrrd)
    if 'spacings' not in nrrd_header or \
        'units' not in nrrd_header:
        print('\'spacings\' or \'units\' not set. Assuming spacing of 1x1x1mm.')
        nrrd_header['spacings'] = [1.0]*3
        nrrd_header['units'] = ['mm']*3

    # create the ctl volume
    volume = ctl.VoxelVolumeF.from_numpy(nrrd_volume)
    volume.set_voxel_size(tuple(nrrd_header['spacings']))

    # use of a predefined system from ctl.blueprints
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericCarmCT())

    # create an acquisition setup
    nb_views = 100
    setup = ctl.AcquisitionSetup(system, nb_views)

    # add a predefined trajectory to the setup from ctl.protocols
    angle_span = ctl.deg2rad(200.0) # rad is the standard unit for angles
    source_to_isocenter = 750.0 # mm is the standard unit for length dimensions
    setup.apply_preparation_protocol(ctl.protocols.WobbleTrajectory(angle_span,
                                                                    source_to_isocenter))
    assert setup.is_valid()

    # configure a projector and project volume
    projector = ctl.ocl.RayCasterProjector()      # the projector (uses its default settings)
    projections = projector.configure_and_project(setup, volume)

    # save to nrrd file (from detector module #0)
    nrrd.write('projections.nrrd', projections.numpy()[:, 0].transpose())
    print('Wrote projections.nrrd')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Create projections of NRRD file.')
    parser.add_argument('nrrd_file', type=str, help='path to an NRRD file')
    args = parser.parse_args()
    main(args.nrrd_file)
