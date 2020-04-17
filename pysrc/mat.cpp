#include <pybind11/pybind11.h>

namespace py = pybind11;

void init_matrix(py::module&);
void init_matrix_utils(py::module&);

void init_mat(py::module& m)
{
    init_matrix(m);
    init_matrix_utils(m);
}
