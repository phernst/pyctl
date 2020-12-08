# PyCTL - Python bindings for the Computed Tomography Library (CTL)

A Python package for simple and interactive use of the [CTL](https://gitlab.com/tpfeiffe/ctl).

Reference publication:  
Tim Pfeiffer, Robert Frysch, Richard N. K. Bismark, and Georg Rose
"CTL: modular open-source C++-library for CT-simulations",
Proc. SPIE 11072,
15th International Meeting on Fully Three-Dimensional Image Reconstruction in Radiology and Nuclear Medicine,
110721L (28 May 2019);
[doi:10.1117/12.2534517](https://doi.org/10.1117/12.2534517)

# Simple installation
Install via pip:
```
pip install pyctl
```
Note: This package was compiled with Qt 5.12. If you have installed Qt<5.12 or >=5.15, please remove any reference
to Qt directories from the `PATH` environment variable or compile and install the package from sources.

# Compiling from sources
The following compilation guide has been tested on Windows 10 with MSVC 2019, CUDA 10.1, Python>=3.6.

## On Windows
* Install [MS Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with Windows SDK.
* Install [CMake](https://cmake.org/download/). Make sure the `PATH` environment variable is set accordingly.
* Install [Git](https://git-scm.com/). Clone this repository: `git clone --recursive https://github.com/phernst/pyctl.git`.
* Install [Qt](https://www.qt.io/)>=5.12. During installation, you need to set at least:
    * Prebuilt components for MSVC 64-bit. Make sure the `PATH` environment variable is set accordingly
      (e.g. `C:\Qt\5.14.1\msvc2017_64\bin`). Make sure the `CMAKE_PREFIX_PATH` environment variable
      is set accordingly (e.g. `C:\Qt\5.14.1\msvc2017_64\lib\cmake\Qt5`).
    * Qt Charts
* Install OpenCL 1.1/1.2:
    * Install latest NVIDIA driver.
    * Install [CUDA](https://developer.nvidia.com/cuda-downloads). Make sure the `CUDA_PATH` and
      `PATH` environment variable are set accordingly.
* Install Python 3 + pip. Recommended: use a virtual [conda environment](https://www.anaconda.com/).
* `cd` into the cloned PyCTL directory and run `pip install .`

## On Linux (not tested)
* Install build tools: `sudo apt install build-essential`
* Install CMake: `sudo apt install cmake`.
* Install Git: `sudo apt install git`. Clone this repository: `git clone --recursive https://github.com/phernst/pyctl.git`.
* Install Qt: `sudo apt install qt5-default qt3d5-dev libqt5charts5-dev`
* Install OpenCL 1.1/1.2:
    * Install official NVIDIA driver using Driver Manager --> reboot
    * Install NVIDIA OpenCL development package: `sudo apt install nvidia-opencl-dev`
    * Install OpenCL headers: `sudo apt install opencl-headers`
* Install Python 3: `sudo apt install python3`. Recommended: use a virtual environment.
* `cd` into the cloned PyCTL directory and run `pip install .`

# Teaser: Making projections

The following example code uses a predefined C-arm system and a predefined
trajectory (a trajectory is a specific acquisition protocol) in order to
project a volume, which is read from a file. This serves to show how the CTL
may work out of the box. However, CT systems or acquisition protocols (or even
preparations of single views) can be freely configured. Moreover, certain
projector extensions can "decorate" the used forward projector in order to
include further geometric/physical/measuring effects.

```python
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

```

If everything works well, you should see this projection:
![example-projection](examples/readme_example.png?raw=true)

If you have any problems or questions regarding the CTL or PyCTL, please contact us:  
<sw4g.production@gmail.com>.
