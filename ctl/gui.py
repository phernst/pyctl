import _ctl

_ctl.gui.init_qapp()

__all__ = [name for name in dir(_ctl.gui) if name[0] != '_' and name != 'init_qapp']


def _add_ctl_attr():
    for name in dir(_ctl.gui):
        if name[0] == '_' or name == 'init_qapp':
            continue
        globals()[name] = getattr(_ctl.gui, name)


_add_ctl_attr()
del _add_ctl_attr

del _ctl
