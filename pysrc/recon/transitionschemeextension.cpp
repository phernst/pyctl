#include "pysrc/pybind_common.h"
#include <recon/transitionschemeextension.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_transitionschemeextension(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<TransitionSchemeExtension, AbstractSubsetGenerator, PySharedPtr<TransitionSchemeExtension>>(m, "TransitionSchemeExtension")
        .def(py::init([](PySharedPtr<AbstractFixedSizeSubsetGenerator> gen)
        {
            return make_pysharedptr<TransitionSchemeExtension>(gen.release_to_ctl());
        }), "nested_generator"_a)
        .def("set_transition_period", &TransitionSchemeExtension::setTransitionPeriod,
            "transition_every_n_iterations"_a)
        .def("set_maximum_nb_subsets", &TransitionSchemeExtension::setMaximumNbSubsets,
            "max_nb_subsets"_a)
        .def("set_minimum_nb_subsets", &TransitionSchemeExtension::setMinimumNbSubsets,
            "min_nb_subsets"_a)
        .def("set_subset_bounds", &TransitionSchemeExtension::setSubsetBounds,
            "initial_nb_subsets"_a, "final_nb_subsets"_a)
        .def("set_subset_generator", [](TransitionSchemeExtension& self, PySharedPtr<AbstractFixedSizeSubsetGenerator> gen)
        {
            self.setSubsetGenerator(gen.release_to_ctl());
        }, "nested_generator"_a);
}
