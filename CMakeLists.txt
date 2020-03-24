cmake_minimum_required(VERSION 3.1.0)

project(pyctl)

add_subdirectory(pybind11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(OpenCL REQUIRED)

set (CTL_DIR "ctl_src/modules/src")

set (CTL_SOURCES
    ${CTL_DIR}/acquisition/acquisitionsetup.cpp
    ${CTL_DIR}/acquisition/ctsystem.cpp
    ${CTL_DIR}/acquisition/ctsystembuilder.cpp
    ${CTL_DIR}/acquisition/geometrydecoder.cpp
    ${CTL_DIR}/acquisition/geometryencoder.cpp
    ${CTL_DIR}/acquisition/preparationprotocols.cpp
    ${CTL_DIR}/acquisition/preparesteps.cpp
    ${CTL_DIR}/acquisition/simplectsystem.cpp
    ${CTL_DIR}/acquisition/trajectories.cpp
    ${CTL_DIR}/acquisition/viewgeometry.cpp
    ${CTL_DIR}/components/carmgantry.cpp
    ${CTL_DIR}/components/cylindricaldetector.cpp
    ${CTL_DIR}/components/flatpaneldetector.cpp
    ${CTL_DIR}/components/genericbeammodifier.cpp
    ${CTL_DIR}/components/genericdetector.cpp
    ${CTL_DIR}/components/genericgantry.cpp
    ${CTL_DIR}/components/genericsource.cpp
    ${CTL_DIR}/components/systemcomponent.cpp
    ${CTL_DIR}/components/tubulargantry.cpp
    ${CTL_DIR}/components/xraylaser.cpp
    ${CTL_DIR}/components/xraytube.cpp
    ${CTL_DIR}/img/chunk2d.tpp
    ${CTL_DIR}/img/compositevolume.cpp
    ${CTL_DIR}/img/projectiondata.cpp
    ${CTL_DIR}/img/singleviewdata.cpp
    ${CTL_DIR}/img/spectralvolumedata.cpp
    ${CTL_DIR}/img/voxelvolume.tpp
    ${CTL_DIR}/io/basetypeio.tpp
    ${CTL_DIR}/io/jsonserializer.cpp
    ${CTL_DIR}/io/serializationhelper.cpp
    ${CTL_DIR}/io/binaryserializer.cpp
    ${CTL_DIR}/io/ctldatabase.cpp
    ${CTL_DIR}/io/messagehandler.cpp
    ${CTL_DIR}/mat/homography.cpp
    ${CTL_DIR}/mat/matrix_utils.tpp
    ${CTL_DIR}/mat/matrix_algorithm.cpp
    ${CTL_DIR}/mat/pmatcomparator.cpp
    ${CTL_DIR}/mat/projectionmatrix.cpp
    ${CTL_DIR}/models/detectorsaturationmodels.cpp
    ${CTL_DIR}/models/intervaldataseries.cpp
    ${CTL_DIR}/models/stepfunctionmodels.cpp
    ${CTL_DIR}/models/tabulateddatamodel.cpp
    ${CTL_DIR}/models/xrayspectrummodels.cpp
    ${CTL_DIR}/models/xydataseries.cpp
    ${CTL_DIR}/processing/errormetrics.cpp
    ${CTL_DIR}/processing/filter.cpp
    ${CTL_DIR}/processing/imageprocessing.cpp
    ${CTL_DIR}/processing/modelbasedvolumedecomposer.cpp
    ${CTL_DIR}/projectors/arealfocalspotextension.cpp
    ${CTL_DIR}/projectors/detectorsaturationextension.cpp
    ${CTL_DIR}/projectors/poissonnoiseextension.cpp
    ${CTL_DIR}/models/abstractdatamodel.cpp
    ${CTL_DIR}/models/datamodeloperations.cpp
    ${CTL_DIR}/components/attenuationfilter.cpp
    ${CTL_DIR}/models/abstractxrayspectrummodel.cpp
    ${CTL_DIR}/acquisition/radiationencoder.cpp
    ${CTL_DIR}/projectors/spectraleffectsextension.cpp
    ${CTL_DIR}/projectors/projectionpipeline.cpp
    ${CTL_DIR}/projectors/projectorextension.cpp
    ${CTL_DIR}/projectors/dynamicprojectorextension.cpp
    ${CTL_DIR}/img/lineardynamicvolume.cpp
    ${CTL_DIR}/mat/matrix.tpp)
	
set (CTL_SOURCES ${CTL_SOURCES}
    ${CTL_DIR}/processing/consistency.cpp
    ${CTL_DIR}/processing/imageresampler.cpp
    ${CTL_DIR}/processing/radontransform2d.cpp
    ${CTL_DIR}/processing/radontransform3d.cpp
    ${CTL_DIR}/projectors/raycaster.cpp
    ${CTL_DIR}/projectors/raycasteradapter.cpp
    ${CTL_DIR}/projectors/raycasterprojector.cpp
    ${CTL_DIR}/processing/volumeresampler.cpp
    ${CTL_DIR}/processing/volumeslicer.cpp
    ${CTL_DIR}/projectors/standardpipeline.cpp)
	
set (CTL_SOURCES ${CTL_SOURCES}
	${CTL_DIR}/ocl/clfileloader.cpp
    ${CTL_DIR}/ocl/openclconfig.cpp
    ${CTL_DIR}/ocl/pinnedmem.cpp)
	
set (MOC_FILES ${CTL_DIR}/projectors/abstractprojector.h)
	
add_compile_definitions(_USE_MATH_DEFINES QT_NO_DEBUG_OUTPUT)

set (PYCTL_SOURCES
    pysrc/pyctl.cpp
    pysrc/ocl.cpp
    pysrc/ocl/clfileloader.cpp
    pysrc/acquisition/acquisitionsetup.cpp
    pysrc/acquisition/ctsystem.cpp
    pysrc/acquisition/ctsystembuilder.cpp
    pysrc/acquisition/trajectories.cpp
    pysrc/acquisition/abstractpreparestep.cpp
    pysrc/projectors/abstractprojector.cpp
    pysrc/projectors/raycasterprojector.cpp
    pysrc/img/chunk2d.cpp
    pysrc/img/singleviewdata.cpp
    pysrc/img/projectiondata.cpp
    pysrc/img/voxelvolume.cpp
    pysrc/img/spectralvolumedata.cpp)

pybind11_add_module(_ctl MODULE ${PYCTL_SOURCES} ${CTL_SOURCES} ${MOC_FILES})
target_include_directories(_ctl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CTL_DIR})
target_link_libraries(_ctl PRIVATE Qt5::Core Qt5::Widgets OpenCL::OpenCL)
