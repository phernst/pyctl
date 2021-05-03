#include "pysrc/pybind_common.h"
#include <acquisition/acquisitionsetup.h>

namespace py = pybind11;

void init_acquisitionsetup(py::module& m)
{
    using namespace CTL;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    auto as { py::class_<AcquisitionSetup, std::shared_ptr<AcquisitionSetup>>(m, "AcquisitionSetup") };
    auto v { py::class_<AcquisitionSetup::View>(as, "View") };

    using AcView = AcquisitionSetup::View;
    as.def(py::init<>())
        .def(py::init<const CTSystem&, uint>(), "system"_a, "nb_views"_a)
        .def(py::init<const AcquisitionSetup&>(), "other"_a)
        .def("add_view", &AcquisitionSetup::addView, "view"_a)
        .def("apply_preparation_protocol", &AcquisitionSetup::applyPreparationProtocol)
        .def("is_valid", &AcquisitionSetup::isValid)
        .def("nb_views", &AcquisitionSetup::nbViews)
        .def("prepare_view", &AcquisitionSetup::prepareView, "view_nb"_a)
        .def("remove_all_prepare_steps", &AcquisitionSetup::removeAllPrepareSteps,
            "keep_time_stamps"_a = false)
        .def("remove_all_prepare_steps_of_type", &AcquisitionSetup::removeAllPrepareStepsOfType,
            "prepare_step_type"_a)
        .def("remove_all_views", &AcquisitionSetup::removeAllViews)
        .def("reset_system", static_cast<bool(AcquisitionSetup::*)(const CTSystem&)>
            (&AcquisitionSetup::resetSystem), "system")
        .def("set_nb_views", &AcquisitionSetup::setNbViews, "nb_views"_a)
        .def("system", static_cast<SimpleCTSystem*(AcquisitionSetup::*)()>(&AcquisitionSetup::system),
            rvp::reference_internal)
        .def("view", static_cast<AcView&(AcquisitionSetup::*)(uint)>
            (&AcquisitionSetup::view), "view_nb"_a, rvp::reference_internal)
        .def("views", static_cast<std::vector<AcquisitionSetup::View>&(AcquisitionSetup::*)()>
            (&AcquisitionSetup::views), rvp::reference_internal);

    using PrepStep = AcquisitionSetup::PrepareStep;
    v.def(py::init<>())
        .def(py::init<double>(), "time")
        .def("set_time_stamp", &AcView::setTimeStamp, "time_stamp"_a)
        .def("time_stamp", &AcView::timeStamp)
        .def("add_prepare_step", &AcView::addPrepareStep, "step"_a)
        .def("clear_prepare_steps", &AcView::clearPrepareSteps)
        .def("nb_prepare_steps", &AcView::nbPrepareSteps)
        .def("prepare_steps", &AcView::prepareSteps, rvp::reference_internal)
        .def("prepare_step", static_cast<const PrepStep&(AcView::*)(int,bool)const>
            (&AcView::prepareStep), "prepare_step_type"_a, "search_from_back"_a = true,
            rvp::reference_internal)
        .def("index_of_prepare_step", &AcView::indexOfPrepareStep, "prepare_step_type"_a,
            "search_from_back"_a = true)
        .def("has_prepare_step", &AcView::hasPrepareStep, "prepare_step_type"_a) // TODO: type
        .def("replace_prepare_step", static_cast<bool(AcView::*)(int,PrepStep)>
            (&AcView::replacePrepareStep), "new_prepare_step"_a, "search_from_back"_a = true)
        .def("remove_all_prepare_steps", &AcView::removeAllPrepareSteps, "prepare_step_type"_a)
        .def("remove_last_prepare_step", &AcView::removeLastPrepareStep)
        .def("remove_prepare_step", &AcView::removePrepareStep, "prepare_step_type"_a,
            "search_from_back"_a = true);
}
