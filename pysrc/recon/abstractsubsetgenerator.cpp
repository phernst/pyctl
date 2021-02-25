#include <pybind11/pybind11.h>
#include <recon/abstractsubsetgenerator.h>
#include <acquisition/acquisitionsetup.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_abstractsubsetgenerator(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    py::class_<AbstractSubsetGenerator, PySharedPtr<AbstractSubsetGenerator>>(m, "AbstractSubsetGenerator")
        .def("set_setup", &AbstractSubsetGenerator::setSetup, "setup"_a)
        .def("set_projections", &AbstractSubsetGenerator::setProjections,
            "projections"_a)
        .def("set_data", &AbstractSubsetGenerator::setData, "projections"_a, 
            "setup"_a)
        .def("generate_subsets", &AbstractSubsetGenerator::generateSubsets,
            "iteration"_a)
        .def("generate_all_subsets", &AbstractSubsetGenerator::generateAllSubsets,
            "nb_iterations"_a)
        .def("is_subset_permutation_enabled", &AbstractSubsetGenerator::isSubsetPermutationEnabled)
        .def("projections", &AbstractSubsetGenerator::projections)
        .def("random_generator_seed", &AbstractSubsetGenerator::randomGeneratorSeed)
        .def("set_random_generator_seed", &AbstractSubsetGenerator::setRandomGeneratorSeed,
            "seed"_a = std::random_device{}())
        .def("set_subset_permutation_enabled", &AbstractSubsetGenerator::setSubsetPermutationEnabled,
            "enabled"_a);

    py::class_<AbstractFixedSizeSubsetGenerator, AbstractSubsetGenerator, PySharedPtr<AbstractFixedSizeSubsetGenerator>>(m, "AbstractFixedSizeSubsetGenerator")
        .def("nb_subsets", &AbstractFixedSizeSubsetGenerator::nbSubsets)
        .def("set_nb_subsets", &AbstractFixedSizeSubsetGenerator::setNbSubsets,
            "nb_subsets");
}
