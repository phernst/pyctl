#include <ocl/clfileloader.h>
#include "pysrc/pybind_common.h"
#include <QString>

namespace py = pybind11;

static std::string opencl_source_dir()
{
    return CTL::ClFileLoader::openCLSourceDir().toStdString();
}

void init_clfileloader(py::module& m)
{
    using namespace CTL;
    py::class_<ClFileLoader>(m, "ClFileLoader")
        .def(py::init<std::string>())
        .def("set_file_name", static_cast<void(ClFileLoader::*)(std::string)>(&ClFileLoader::setFileName))
        .def("file_name", &ClFileLoader::fileName)
        .def("is_valid", &ClFileLoader::isValid)
        .def("load_source_code", &ClFileLoader::loadSourceCode)
        .def_static("set_opencl_source_dir", static_cast<void(*)(const std::string&)>(&ClFileLoader::setOpenCLSourceDir))
        .def_static("opencl_source_dir", &opencl_source_dir);
}