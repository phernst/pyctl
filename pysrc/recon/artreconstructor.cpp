#include "pysrc/pybind_common.h"
#include <recon/artreconstructor.h>
#include "../pysharedptr.h"

namespace py = pybind11;

void init_artreconstructor(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;

    auto ar = py::class_<ARTReconstructor, AbstractReconstructor, PySharedPtr<ARTReconstructor>>(m, "ARTReconstructor");
    py::enum_<ARTReconstructor::StoppingCriterion>(ar, "StoppingCriterion", py::arithmetic())
        .value("NoStoppingCriterion", ARTReconstructor::NoStoppingCriterion)
        .value("MaximumNbIterations", ARTReconstructor::MaximumNbIterations)
        .value("MaximumTime", ARTReconstructor::MaximumTime)
        .value("ProjectionErrorChange", ARTReconstructor::ProjectionErrorChange)
        .value("VolumeDomainChange", ARTReconstructor::VolumeDomainChange)
        .value("RelativeProjectionError", ARTReconstructor::RelativeProjectionError)
        .value("NormalEquationSatisfied", ARTReconstructor::NormalEquationSatisfied)
        .value("AllStoppingCriteria", ARTReconstructor::AllStoppingCriteria)
        .export_values();

    ar.def(py::init<>())
        .def(py::init([](PySharedPtr<AbstractProjector> forward, PySharedPtr<AbstractReconstructor> backward)
        {
            return make_pysharedptr<ARTReconstructor>(forward.release_to_ctl(), backward.release_to_ctl());
        }), "forward_projector"_a, "backprojector"_a)
        .def_static("compute_relaxation_estimate", static_cast<float(*)(const AcquisitionSetup&,
            const VoxelVolume<float>&, AbstractProjector*, AbstractReconstructor*, uint)>
            (ARTReconstructor::computeRelaxationEstimate), "setup"_a, "target_volume"_a,
            "forward_projector"_a = nullptr, "backprojector"_a = nullptr, "nb_power_iter"_a = 1u)
        .def("set_positivity_constraint_enabled", &ARTReconstructor::setPositivityConstraintEnabled,
            "enabled"_a)
        .def("set_regularization_enabled", &ARTReconstructor::setRegularizationEnabled,
            "enabled"_a)
        .def("set_relaxation_estimation_enabled", &ARTReconstructor::setRelaxationEstimationEnabled,
            "enabled"_a)
        .def("set_relaxation", &ARTReconstructor::setRelaxation, "relax"_a)
        .def("set_relaxation_by_estimation", static_cast<void(ARTReconstructor::*)
            (const VoxelVolume<float>&,uint)>(&ARTReconstructor::setRelaxationByEstimation),
            "target_volume"_a, "nb_power_iter"_a = 1u)
        .def("set_relaxation_by_estimation", static_cast<void(ARTReconstructor::*)
            (const AcquisitionSetup&,const VoxelVolume<float>&,uint)>
            (&ARTReconstructor::setRelaxationByEstimation),
            "setup"_a, "target_volume"_a, "nb_power_iter"_a = 1u)
        .def("set_regularizer", [](ARTReconstructor& self, PySharedPtr<AbstractVolumeFilter> reg, bool en)
        {
            self.setRegularizer(reg.release_to_ctl(), en);
        }, "regularizer"_a, "enable_regularizer"_a = true)
        .def("set_subset_generator", [](ARTReconstructor& self, PySharedPtr<AbstractSubsetGenerator>& gen)
        {
            self.setSubsetGenerator(gen.release_to_ctl());
        }, "generator"_a)
        .def("set_forward_projector", [](ARTReconstructor& self, PySharedPtr<AbstractProjector>& p)
        {
            self.setForwardProjector(p.release_to_ctl());
        }, "projector"_a)
        .def("set_backprojector", [](ARTReconstructor& self, PySharedPtr<AbstractReconstructor>& r)
        {
            self.setBackprojector(r.release_to_ctl());
        }, "projector"_a)
        .def("use_custom_subset_generator", &ARTReconstructor::useCustomSubsetGenerator)
        .def("use_default_subset_generator", &ARTReconstructor::useDefaultSubsetGenerator)
        .def("set_max_nb_iterations", &ARTReconstructor::setMaxNbIterations, "nb_it"_a,
            "enable_criterion"_a = true)
        .def("set_max_time", &ARTReconstructor::setMaxTime, "seconds"_a,
            "enable_criterion"_a = true)
        .def("set_min_change_in_volume_domain", &ARTReconstructor::setMinChangeInVolumeDomain,
            "min_relative_change"_a, "enable_criterion"_a = true)
        .def("set_min_change_in_projection_error", &ARTReconstructor::setMinChangeInProjectionError,
            "min_relative_change"_a, "enable_criterion"_a = true)
        .def("set_min_relative_projection_error", &ARTReconstructor::setMinRelativeProjectionError,
            "min_relative_error"_a, "enable_criterion"_a = true)
        .def("set_normal_eq_tolerance", &ARTReconstructor::setNormalEqTolerance,
            "relative_tol"_a, "enable_criterion"_a = true)
        .def("set_stopping_criteria", &ARTReconstructor::setStoppingCriteria, "criteria"_a)
        .def("set_stopping_criterion_enabled", &ARTReconstructor::setStoppingCriterionEnabled,
            "criterion"_a, "enabled"_a = true)
        .def("is_custom_subset_generator_in_use", &ARTReconstructor::isCustomSubsetGeneratorInUse)
        .def("is_default_subset_generator_in_use", &ARTReconstructor::isDefaultSubsetGeneratorInUse)
        .def("is_positivity_constraint_enabled", &ARTReconstructor::isPositivityConstraintEnabled)
        .def("is_relaxation_estimation_enabled", &ARTReconstructor::isRelaxationEstimationEnabled)
        .def("is_regularization_enabled", &ARTReconstructor::isRegularizationEnabled)
        .def("is_stopping_criterion_enabled", &ARTReconstructor::isStoppingCriterionEnabled,
            "criteria"_a)
        .def("relaxation", &ARTReconstructor::relaxation)
        .def("stopping_criteria", &ARTReconstructor::stoppingCriteria)
        .def("regularizer", &ARTReconstructor::regularizer)
        .def("custom_subset_generator", &ARTReconstructor::customSubsetGenerator)
        .def("default_subset_generator", &ARTReconstructor::defaultSubsetGenerator)
        .def("subset_generator", &ARTReconstructor::subsetGenerator);
}
