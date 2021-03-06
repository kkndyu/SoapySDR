########################################################################
# Project setup
########################################################################
cmake_minimum_required(VERSION 2.8)
project(SoapySDRPython3 CXX)
enable_testing()

find_package(SoapySDR NO_MODULE REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

########################################################################
# Find SWIG
########################################################################
find_package(SWIG)
message(STATUS "SWIG_FOUND: ${SWIG_FOUND} - ${SWIG_VERSION}")

########################################################################
# Find python interp
########################################################################
find_package(Python3Interp)
message(STATUS "PYTHON3INTERP_FOUND: ${PYTHON3INTERP_FOUND}")
message(STATUS "PYTHON3_EXECUTABLE: ${PYTHON3_EXECUTABLE}")

########################################################################
# Determine install directory
########################################################################
execute_process(
    COMMAND ${PYTHON3_EXECUTABLE} -c
    "from distutils.sysconfig import get_python_lib; print(get_python_lib(plat_specific=True, prefix=''))"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON3_INSTALL_DIR
)
set(PYTHON3_INSTALL_DIR ${PYTHON3_INSTALL_DIR} CACHE STRING "python3 install prefix")
message(STATUS "PYTHON3_INSTALL_DIR: \${prefix}/${PYTHON3_INSTALL_DIR}")

########################################################################
# Find Python libs
########################################################################
find_package(Python3Libs)
message(STATUS "PYTHON3LIBS_FOUND: ${PYTHON3LIBS_FOUND}")
message(STATUS "PYTHON3_INCLUDE_DIRS: ${PYTHON3_INCLUDE_DIRS}")
message(STATUS "PYTHON3_LIBRARIES: ${PYTHON3_LIBRARIES}")

#on windows, we require a pythonxx_d.lib in debug mode
#require that the PYTHON_DEBUG_LIBRARY flag is set
#or the build assumes that the debug library DNE
set(PYTHON3_DEBUG_OK TRUE)
if(WIN32 AND NOT PYTHON3_DEBUG_LIBRARY AND "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(WARNING "WIN32 Debug mode requires PYTHON3_DEBUG_LIBRARY")
    set(PYTHON3_DEBUG_OK FALSE)
endif()

########################################################################
## Feature registration
########################################################################
include(FeatureSummary)
include(CMakeDependentOption)
cmake_dependent_option(ENABLE_PYTHON3 "Enable python bindings" ON "ENABLE_LIBRARY;SWIG_FOUND;PYTHON3INTERP_FOUND;PYTHON3LIBS_FOUND;PYTHON3_DEBUG_OK" OFF)
add_feature_info(Python3 ENABLE_PYTHON3 "python3 bindings")
if (NOT ENABLE_PYTHON3)
    return()
endif()

########################################################################
# Build Module
########################################################################
include(UseSWIG)
include_directories(${SoapySDR_INCLUDE_DIRS})
include_directories(${PYTHON3_INCLUDE_DIRS})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../python/SoapySDR.i
    ${CMAKE_CURRENT_BINARY_DIR}/SoapySDR.i
@ONLY)

message(STATUS "CMAKE_SWIG_FLAGS=${CMAKE_SWIG_FLAGS}")
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/SoapySDR.i PROPERTIES CPLUSPLUS ON)

SWIG_ADD_MODULE(SoapySDR3 python ${CMAKE_CURRENT_BINARY_DIR}/SoapySDR.i)
SWIG_LINK_LIBRARIES(SoapySDR3 ${SoapySDR_LIBRARIES} ${PYTHON3_LIBRARIES})

set_target_properties(${SWIG_MODULE_SoapySDR3_REAL_NAME} PROPERTIES OUTPUT_NAME _SoapySDR)

########################################################################
# Install Module
########################################################################
install(
    TARGETS ${SWIG_MODULE_SoapySDR3_REAL_NAME}
    DESTINATION ${PYTHON3_INSTALL_DIR}
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/SoapySDR.py
    DESTINATION ${PYTHON3_INSTALL_DIR}
)
