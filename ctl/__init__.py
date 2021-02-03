from pkg_resources import resource_filename
import ctypes
from ctypes.util import find_library
import sys

def _raise_if_wrong_env_qt():
    qt_lib_path = find_library('Qt5Core')
    if qt_lib_path is None:
        return
    qt5core = ctypes.CDLL(qt_lib_path)
    qt5core.qVersion.argtypes = []
    qt5core.qVersion.restype = ctypes.c_char_p
    qtver = ctypes.c_char_p(qt5core.qVersion()).value.decode()
    raise RuntimeError(f'Qt5 version must be >=5.15 (is {qtver}). '
                       'Consider removing Qt from your environment paths.')

try:
    import _ctl
except ImportError:
    _raise_if_wrong_env_qt()

    from PySide2.QtWidgets import QApplication
    QApplication(sys.argv)
    del QApplication
    import _ctl

del _raise_if_wrong_env_qt
del sys
del find_library
del ctypes

cl_source_dir = resource_filename('ctl', 'cl_src')
db_source_dir = resource_filename('ctl', 'database')

_ctl.ocl.ClFileLoader.set_opencl_source_dir(cl_source_dir)
_ctl.CTLDatabaseHandler.set_database_root(db_source_dir)
del cl_source_dir
del db_source_dir

__all__ = [name for name in dir(_ctl) if name[0] != '_' and name != 'gui']

def _add_ctl_attr():
    for name in dir(_ctl):
        if name[0] == '_' or name == 'gui':
            continue
        globals()[name] = getattr(_ctl, name)
_add_ctl_attr()
del _add_ctl_attr

del _ctl
