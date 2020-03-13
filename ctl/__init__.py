import os
from os.path import join as pjoin

import _ctl

_ctl.ocl.ClFileLoader.set_opencl_source_dir(pjoin(os.path.dirname(os.path.abspath(__file__)), 'cl_src'))
