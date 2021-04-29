#include "pysrc/pybind_common.h"
#include <projectors/poissonnoiseextension.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_poissonnoiseextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<PoissonNoiseExtension, ProjectorExtension, PySharedPtr<PoissonNoiseExtension>>(m, "PoissonNoiseExtension")
        .def(py::init<>())
        .def(py::init<uint, bool>(), "fixed_seed"_a, "use_parallelization"_a = true)
        .def("is_linear", &PoissonNoiseExtension::isLinear)
        .def("set_fixed_seed", &PoissonNoiseExtension::setFixedSeed, "seed"_a)
        .def("set_random_seed_mode", &PoissonNoiseExtension::setRandomSeedMode)
        .def("set_parallelization_enabled", &PoissonNoiseExtension::setParallelizationEnabled,
            "enabled"_a)
        .def("configure", &PoissonNoiseExtension::configure, "setup"_a, py::keep_alive<1,2>());
}
