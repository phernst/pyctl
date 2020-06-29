import ctl
import nrrd

IN_VOLUME_PATH = "path/to/volume.nrrd"
PROJECTION_MATRICES_PATH = "path/to/projection_matrices.nrrd"
OUT_PROJECTIONS_PATH = "path/to/projection_output.nrrd"

def create_projection_matrices():
    # create a tubular system with a flat panel detector
    system = ctl.CTSystemBuilder.create_from_blueprint(ctl.blueprints.GenericTubularCT())
    system.remove_component(system.detectors()[0])
    system.add_component(ctl.FlatPanelDetector((640, 480), (1.0, 1.0)))

    # create an acquisition setup
    setup = ctl.AcquisitionSetup(system, 10)
    setup.apply_preparation_protocol(ctl.protocols.AxialScanTrajectory())

    # encode the setup and save the projection matrices
    proj_mats = ctl.GeometryEncoder.encode_full_geometry(setup)
    nrrd.write(PROJECTION_MATRICES_PATH,
               proj_mats.numpy().transpose(),
               header={'encoding': 'raw'})
    print("Wrote projection matrices")

def load_projection_matrices():
    nrrd_pmats, _ = nrrd.read(PROJECTION_MATRICES_PATH)
    return ctl.FullGeometry.from_numpy(nrrd_pmats.transpose())

def main():
    # load volume
    nrrd_volume, nrrd_header = nrrd.read(IN_VOLUME_PATH)
    volume = ctl.VoxelVolumeF.from_numpy(nrrd_volume.transpose())
    volume.set_voxel_size(tuple(nrrd_header.get('spacings', [1.0]*3)))
    print(volume.voxel_size())

    # independently create and load projection matrices
    create_projection_matrices()
    proj_mats = load_projection_matrices()

    # decode an acquisition setup
    nb_detector_pixels_per_module = (640, 480)
    setup = ctl.GeometryDecoder.decode_full_geometry(proj_mats, nb_detector_pixels_per_module)

    # configure a projector and project volume
    projector = ctl.ocl.RayCasterProjector()
    projections = projector.configure_and_project(setup, volume)

    # save projections
    nrrd.write(OUT_PROJECTIONS_PATH, projections.numpy().transpose())

if __name__ == "__main__":
    main()
