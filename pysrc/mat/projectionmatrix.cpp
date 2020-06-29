#include <pybind11/pybind11.h>
#include <mat/projectionmatrix.h>

namespace py = pybind11;

void init_projectionmatrix(py::module& m)
{
    using namespace CTL::mat;
    using namespace py::literals;
    using rvp = py::return_value_policy;

    const static Matrix<3,1> zero{ 0.0 };

    auto pm = py::class_<ProjectionMatrix, Matrix<3,4>>(m, "ProjectionMatrix");
    auto nm = py::enum_<ProjectionMatrix::NormalizationMode>(pm, "NormalizationMode");
    
    pm.def(py::init<>())
        .def(py::init<Matrix<3,4>>(), "other"_a)
        .def_static("compose", [](const Matrix<3,3>& M, const Matrix<3,1>& p4)
        {
            return ProjectionMatrix::compose(M, p4);
        }, "m"_a, "p4"_a)
        .def_static("compose", [](const Matrix<3,3>& K,
                                  const Matrix<3,3>& R,
                                  const Matrix<3,1>& source)
        {
            return ProjectionMatrix::compose(K, R, source);
        }, "k"_a, "r"_a, "p4"_a)
        .def("shift_detector_origin", static_cast<void(ProjectionMatrix::*)(const Matrix<2,1>&)>
            (&ProjectionMatrix::shiftDetectorOrigin), "translation"_a)
        .def("shift_detector_origin", static_cast<void(ProjectionMatrix::*)(double,double)>
            (&ProjectionMatrix::shiftDetectorOrigin), "translation_x"_a, "translation_y"_a)
        .def("change_detector_resolution", static_cast<void(ProjectionMatrix::*)(double)>
            (&ProjectionMatrix::changeDetectorResolution), "resampling_factor"_a)
        .def("change_detector_resolution", static_cast<void(ProjectionMatrix::*)(double,double)>
            (&ProjectionMatrix::changeDetectorResolution), "resampling_factor_x"_a,
            "resampling_factor_y"_a)
        .def("normalize", &ProjectionMatrix::normalize)
        .def("normalized", &ProjectionMatrix::normalized)
        .def("m", &ProjectionMatrix::M)
        .def("p4", &ProjectionMatrix::p4)
        .def("direction_source_to_pixel", [](const ProjectionMatrix& self,
                                             const Matrix<2,1>& pc,
                                             ProjectionMatrix::NormalizationMode nm)
        {
            return self.directionSourceToPixel(pc, nm);
        }, "pixel_coordinates"_a, "normalization_mode"_a = ProjectionMatrix::NoNormalization)
        .def("direction_source_to_pixel", [](const ProjectionMatrix& self,
                                             double x,
                                             double y,
                                             ProjectionMatrix::NormalizationMode nm)
        {
            return self.directionSourceToPixel(x, y, nm);
        }, "x"_a, "y"_a, "normalization_mode"_a = ProjectionMatrix::NoNormalization)
        .def("principal_ray_direction", &ProjectionMatrix::principalRayDirection)
        .def("translation_cts", &ProjectionMatrix::translationCTS)
        .def("source_position", &ProjectionMatrix::sourcePosition)
        .def("rotation_mat_r", &ProjectionMatrix::rotationMatR)
        .def("intrinsic_mat_k", &ProjectionMatrix::intrinsicMatK)
        .def("principal_point", &ProjectionMatrix::principalPoint)
        .def("focal_length", &ProjectionMatrix::focalLength)
        .def("skew_coefficient", &ProjectionMatrix::skewCoefficient)
        .def("magnification_x", [](const ProjectionMatrix& self, double x, double y, double z)
        {
            return self.magnificationX(x, y, z);
        }, "x"_a, "y"_a, "z"_a)
        .def("magnification_x", [](const ProjectionMatrix& self, const Matrix<3,1>& wc)
        {
            return self.magnificationX(wc);
        }, "world_coordinate"_a = zero)
        .def("magnification_y", [](const ProjectionMatrix& self, double x, double y, double z)
        {
            return self.magnificationY(x, y, z);
        }, "x"_a, "y"_a, "z"_a)
        .def("magnification_y", [](const ProjectionMatrix& self, const Matrix<3,1>& wc)
        {
            return self.magnificationY(wc);
        }, "world_coordinate"_a = zero)
        .def("project_onto_detector", [](const ProjectionMatrix& self,
                                         double x,
                                         double y,
                                         double z)
        {
            return self.projectOntoDetector(x, y, z);
        }, "x"_a, "y"_a, "z"_a)
        .def("project_onto_detector", [](const ProjectionMatrix& self, const Matrix<3,1>& wc)
        {
            return self.projectOntoDetector(wc);
        }, "world_coordinate"_a = zero);


    nm.value("NoNormalization", ProjectionMatrix::NormalizationMode::NoNormalization)
        .value("NormalizeAsUnitVector", ProjectionMatrix::NormalizationMode::NormalizeAsUnitVector)
        .value("NormalizeByX", ProjectionMatrix::NormalizationMode::NormalizeByX)
        .value("NormalizeByY", ProjectionMatrix::NormalizationMode::NormalizeByY)
        .value("NormalizeByChannel", ProjectionMatrix::NormalizationMode::NormalizeByChannel)
        .value("NormalizeByRow", ProjectionMatrix::NormalizationMode::NormalizeByRow)
        .export_values();
}
