try:
    from importlib.resources import path
    cl_path = path('ctl', 'cl_src')
    db_path = path('ctl', 'database')
    del path
except ImportError:
    from importlib_resources import files
    cl_path = files('ctl') / 'cl_src'
    db_path = files('ctl') / 'database'
    del files
import atexit
from contextlib import ExitStack
import _ctl

_file_manager = ExitStack()
atexit.register(_file_manager.close)
cl_source_dir = _file_manager.enter_context(cl_path)
db_source_dir = _file_manager.enter_context(db_path)

del ExitStack
del atexit
del cl_path
del db_path

_ctl.ocl.ClFileLoader.set_opencl_source_dir(str(cl_source_dir.resolve()))
_ctl.CTLDatabaseHandler.set_database_root(str(db_source_dir.resolve()))
del cl_source_dir
del db_source_dir

__all__ = [name for name in dir(_ctl) if name[0] != '_']

for name in dir(_ctl):
    if name[0] == '_':
        continue
    globals()[name] = getattr(_ctl, name)

del _ctl
