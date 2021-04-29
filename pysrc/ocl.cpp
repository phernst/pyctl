#include "pysrc/pybind_common.h"

namespace py = pybind11;

void init_clfileloader(py::module&);
void init_genericoclprojectionfilter(py::module&);
void init_oclprojectionfilters(py::module&);
void init_raycasterprojector(py::module&);
void init_backprojectorbase(py::module&);
void init_sfpprojector(py::module&);
void init_fdkreconstructor(py::module&);
void init_simplebackprojector(py::module&);
void init_sfpbackprojector(py::module&);

void init_ocl(py::module& m)
{
    init_clfileloader(m);
    init_genericoclprojectionfilter(m);
    init_oclprojectionfilters(m);
    init_raycasterprojector(m);
    init_backprojectorbase(m);
    init_sfpprojector(m);
    init_fdkreconstructor(m);
    init_simplebackprojector(m);
    init_sfpbackprojector(m);
}
