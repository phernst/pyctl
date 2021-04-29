#include "pysrc/pybind_common.h"
#include <QApplication>
#include <pysrc/qt_helper.h>

namespace py = pybind11;

void init_volumeviewer(py::module&);
void init_intersectionplaneview(py::module&);
void init_volumesliceviewer(py::module&);
void init_ctsystemview(py::module&);
void init_datamodelviewer(py::module&);
void init_acquisitionsetupview(py::module&);
void init_pipelinecomposer(py::module&);
void init_chunk2dview(py::module&);
void init_projectionviewer(py::module&);

void init_gui(py::module& m)
{
    m.def("init_qapp", []() { qAppInstance(); });
    m.def("show", []()
    {
        if(FigureManager::instance().empty())
        {
            return;
        }
        
        FigureManager::instance().showAllAndClear();
        qAppInstance()->exec();
    });

    init_volumeviewer(m);
    init_intersectionplaneview(m);
    init_volumesliceviewer(m);
    init_ctsystemview(m);
    init_datamodelviewer(m);
    init_acquisitionsetupview(m);
    init_pipelinecomposer(m);
    init_chunk2dview(m);
    init_projectionviewer(m);
}
